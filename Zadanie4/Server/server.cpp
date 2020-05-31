#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <signal.h>

#define MAXLEN 1024

using namespace std;

class Socket {
	int serverSocket;
	public:
		Socket() {
			if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
				throw "Error: creating socket";
			}
		}
		
		int getnum() const {return serverSocket;}
};

class Server: public Socket{
	struct sockaddr_in SockAddress;
	public:
		Server(int port): Socket() {
			memset(&SockAddress, 0, sizeof(SockAddress));
			SockAddress.sin_family = AF_INET;
			SockAddress.sin_addr.s_addr = INADDR_ANY;
			SockAddress.sin_port = htons(port);
		}
		
		void Bind() {
			if (bind(getnum(), (struct sockaddr *) &SockAddress, sizeof (SockAddress)) < 0) {
				close(getnum());
				throw "Error: binding socket";
			}
		}
		void Listen() const {
			if (listen(getnum(), 1) < 0) {
				close(getnum());
				throw "Error while listening socket";
			}
		}
};

class Client {
	struct sockaddr_in ClientAddress;
	socklen_t ClAddrLen;
	int clSocket, fd;
	
	bool ifcgi(char *buf) {
		return (strncmp(buf, "cgi-bin", 7) == 0);
	}
	char* getenv(char* buf);
	char* getreq(char* buf) const;
	char* getname(char* path) const;
	
	public:
		Client(){
			ClAddrLen = sizeof(ClientAddress);
		}
		
		void Wait(Server S){
			if ((clSocket = accept(S.getnum(), (struct sockaddr *) &ClientAddress, &ClAddrLen)) < 0){
				close(S.getnum());
				throw "Error: accepting socket";
			}
		}
		
		void ProcessClientRequest (Server S, char* buf);
};

char* Client::getenv(char* buf) {
	char* env = new char[MAXLEN];
	int i = strlen(buf)-1, k = 0;
	while(buf[i] != '?'){
		if(buf[i--] == 0) {env[0] = 0; return env;}
	}
	for(unsigned j = i+1; j<strlen(buf); j++) env[k++] = buf[j];
	return env;
}

char* Client::getreq(char* buf) const {
	char* str = new char[MAXLEN];
	strcpy(str, buf);
	int f = 0;
	unsigned int i = 0;
	while(i < strlen(buf) && !f){
		if (str[i++] == '?') f = 1;
	}
	if (f) i--;
	str[i] = 0;
	return str;
}

char* Client::getname(char* buf) const {
	char* str = new char[MAXLEN];
	unsigned int i = 0, k = 0;
	while(buf[i++] != '/');
	while (i < strlen(buf)) str[k++] = buf[i++];
	str[k] = '\0';
	str = getreq(str);
	return str;
}

int Soc;

void Client::ProcessClientRequest (Server S, char* buf) {
	Soc = clSocket;
	char f_name[MAXLEN];
	
	if (recv(clSocket, buf, MAXLEN, 0) < 0) {
		if (shutdown(clSocket, 1) < 0) 
			if (shutdown(clSocket, 1) < 0)
				if (shutdown(clSocket, 1) < 0)
					throw "Error: shutdown";
		close (clSocket);
		throw "Error: receiving";
	}
	if (buf[0]==0)
		return;
	cout << ">REQUEST" << endl;
	cout << buf <<"-------------------"<< endl;
	
	int len;
	if (!strncmp(buf, "GET /", 5)){				//если запрос не GET
		int i = 5;
		while (buf[i]!= ' ' && buf[i])
			i++;
		if ((strncmp (buf + i-5, ".html", 5) == 0 )||(strncmp (buf + i-5, ".cpp", 5) == 0 )||(strncmp (buf + i-2, ".c", 5) == 0 )||(strncmp (buf + i-2, ".o", 5) == 0 )){
//файлы с расширениями html, cpp, c, o мы отправлять не будем, к ним нет доступа
			cout << "Error 403" << endl;
			fd = open ("Error403.html", O_RDONLY);
			strcpy(buf, "HTTP/1.1 503 Forbidden\nAllow: GET\nServer: Fenric/0.1\nConnection: keep-alive\nContent-length: ");
		}else{		
		bool h = true;
		char path[i-3];
		if (i == 5) {
			path[0] = '/';
			path[1] = 0;
		} else {
			strncpy(path, buf+5, i-5);
			path[i-5] = 0;
			h = false;
		}
		
		if (ifcgi(path)) {						//если cgi запрос
			int pid, status;
			char name[MAXLEN];
			f_name[0]=0;
			for (int i = 0; i < MAXLEN; i++)	//формируем имя файла
				name[i] = 0;
			sprintf (name, "%d", getpid());
			strcat (name, ".txt");			
			
			if((pid = fork()) < 0)
				throw "Error: fork";
			// сын
			if (pid == 0){						
				chdir("./cgi-bin");				// устанавливает директорию
				fd = open(name, O_WRONLY|O_CREAT|O_TRUNC, 0666);//создаёт файл
				char proc[strlen(name)+3];		//proc="./<path>"
				proc[0] = '.';
				proc[1] = '/';
				proc[2] = 0;
				strcat(proc, getname(path));
				strcpy(buf, "QUERY_STRING=");	//buf="QUERY_STRING=<env>"
				strcat(buf, getenv(path));
				char *argv[] = {(char*) proc, NULL};//пихаем то, что нужно новому процессу
				char *env[] = {(char*) buf, NULL};
				dup2(fd, 1);						//перенаправляем вывод в файл
				execve(proc, argv, env);			//запускаем процесс
				delete [] argv[0];
				delete [] env[0];
				throw "Error: Exec";
			}
			// отец
			wait(&status);
			close(fd);
			if (WIFEXITED(status)){
				if (WEXITSTATUS(status) == 0) {
					char b[strlen(name)+1];			//name="cgi-bin/<name>"
					strcpy(b, name);
					strcpy(name, "cgi-bin/");
					strcat(name, b);
					strcpy(f_name, name);
					fd = open (name, O_RDONLY);		//открываем полученный файл на чтение
					strcpy(buf, "HTTP/1.1 200 ServerFenric\nAllow: GET\nServer: Fenric/0.1\nConnection: keep-alive\nContent-type: text/plain\nContent-length: ");
						//в буфер выводим успех
				}
				else {
					cout<< "CGI finished with status " << WEXITSTATUS(status) << endl;
					fd = open ("Error500.html", O_RDONLY);	//неопознанная ошибка
					strcpy(buf, "HTTP/1.1 500 ServerFenric\nAllow: GET\nServer: Fenric/0.1\nConnection: keep-alive\nContent-type: text/plain\nContent-length: ");
				}
            }else if(WIFSIGNALED(status)) {
				cout << "CGI sent signal " << WIFSIGNALED(status) << endl;
				fd = open ("Error500.html", O_RDONLY);
				strcpy(buf, "HTTP/1.1 500 ServerFenric\nAllow: GET\nServer: Fenric/0.1\nConnection: keep-alive\nContent-type: text/plain\nContent-length: ");
			}
		}
		else {
			if (((fd = open(path, O_RDONLY)) < 0) && !h) {	//файл не открылся, путь был не пустым
				cout << "Error 404\n";		//ошибка 404
				fd = open ("Error404.html", O_RDONLY);
				strcpy(buf, "HTTP/1.1 404 PageNotFound\nAllow: GET\nServer: Fenric/0.1\nConnection: keep-alive\nContent-type: text/html\nContent-length: ");
			}else if (h){		//путь был пустым, открываем страницу по умолчанию
				fd = open("index.html", O_RDONLY);
				strcpy(buf, "HTTP/1.1 200 ServerFenric\nAllow: GET\nServer: Fenric/0.1\nConnection: keep-alive\nContent-type: text/html\nContent-length: ");
			}else											//файл открылся корректно
				strcpy(buf, "HTTP/1.1 200 ServerFenric\nAllow: GET\nServer: Fenric/0.1\nConnection: keep-alive\nContent-type: text/plain\nContent-length: ");
		}
	}
	}else{						//запрос другого рода (не GET??)
		cout << "Error 501\n";
		fd = open ("Error501.html", O_RDONLY);
		strcpy(buf, "HTTP/1.1 501 NotImplemented\nAllow: GET\nServer: Fenric/0.1\nConnection: keep-alive\nContent-length: ");
	}
	
	len = lseek(fd, 0, SEEK_END);	//подсчитываем длину файла
	lseek(fd, 0, 0);				//устанавливаем указатель на начало
	
	char s[11];						//s=<len>
	sprintf (s, "%d", len);
	strcat (buf, s);
	strcat (buf, "\n\n");			//buf=<s>\n\n
	len = strlen(buf);				//len= длина buf
	
	cout << ">ANSWER" << endl;
	cout << buf <<"-------------------"<< endl;
	
	send(clSocket, buf, len, 0);	//отправляем ответ
	while ((len = read(fd, buf, MAXLEN)) > 0)
		send(clSocket, buf, len, 0);
	close(fd);						//отправляем файл
	if (f_name[0]!=0)
		remove(f_name);
	if (shutdown(clSocket, 1) < 0) 		//закрываем соединение
		if (shutdown(clSocket, 1) < 0)
			if (shutdown(clSocket, 1) < 0)
				throw "Error: shutdown";

	close (clSocket);
}

//Обработчики сигналов
void SigHndlr(int i) {
    cout << "\nSigHndlr\n";
    shutdown(Soc, 0);
    close(Soc);
    signal(SIGINT,SIG_DFL); 
}

void SigChldr(int i) {
    int status;
    wait(&status);
    signal(SIGCHLD,SigChldr); 
}
//Мэйн
int main(int argc, char * argv[]){
	try{
		char buf[MAXLEN];
		int port;
		if (argc<2)
			return 0;
		port=atoi(argv[1]);
		
		signal(SIGINT,SigHndlr);
		signal(SIGCHLD,SigChldr);
		
		Server S(port);
		S.Bind();
		S.Listen();
		while(1) {
			for (int i = 0; i < MAXLEN; i++)		//обнуляем буфер
				buf[i] = 0;
			Client C;
			C.Wait(S);
			C.ProcessClientRequest(S, buf);
		}
		return 0;
	}catch(char const *sourse){
		cout << "throwned "<< sourse << endl;
		//throw;
		exit(1);
	}
}
