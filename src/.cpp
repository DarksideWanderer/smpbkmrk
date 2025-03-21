#include<bits/stdc++.h>

template<typename... Args>
std::istream& InPut(Args&...x){return (std::cin>>...>>x);}
template<typename... Args>
std::ostream& OutPut(const Args&...x){return (std::cout<<...<<x);}
template<typename...Args>
std::ostream& ErrPut(const Args&...x){return (std::cerr<<...<<x);}
template<typename T>void Flush(T&x){x.flush();}

template<typename ioStream,typename...Args>
ioStream& WqyInPut(ioStream&t,Args&...x){(t>>...>>x);return t;}
template<typename ioStream,typename...Args>
ioStream& WqyOutPut(ioStream&t,const Args&...x){(t<<...<<x);return t;}

void createFile(std::string path){
	std::ofstream A(path);
	return ;
}

class DataPoint{
public:
	std::string name;
	DataPoint(std::string name):name(name){}
};

class Application{
private:
	std::vector<DataPoint>pla;
	std::filesystem::path target;
	void Init(){
		std::ifstream cach;
		#ifdef __APPLE__
		cach.open("/Users/piggy/.smplbkmrks/cache.txt",std::ios::in);
		if(!cach){
			std::filesystem::create_directories("/Users/piggy/.smplbkmrks");
			std::filesystem::create_directories("/Users/piggy/.smplbkmrks/basic");
			target="/Users/piggy/.smplbkmrks/basic";
			createFile("/Users/piggy/.smplbkmrks/cache.txt");
			return ;
		}
		#elif __WIN__
		#elif __LINUX__
		#endif
		WqyInPut(cach,target);
		std::string Q;
		while(WqyInPut(cach,Q))pla.push_back(Q);
		cach.close();
	}
	void BackUp(){
		std::ofstream cach;
		#ifdef __APPLE__
		cach.open("/Users/piggy/.smplbkmrks/cache.txt",std::ios::out);
		if(!cach){
			std::filesystem::create_directories("/Users/piggy/.smplbkmrks");
			cach.open("/Users/piggy/.smplbkmrks/cache.txt",std::ios::out);
		}
		#endif
		if(cach)std::cerr<<"OK"<<'\n';
		WqyOutPut(cach,target);
		for(auto &t:pla)WqyOutPut(cach,t.name);
		cach.close();
	}
	Application(){Init();}
	~Application(){BackUp();}
	
public:
	static Application* getInstance(){
		static Application instance;
		return &instance;
	}
	void setTarget(const std::string&tar){//tar只能是绝对路径
		//搬运
		if(!std::filesystem::exists(std::filesystem::path(tar))){
			std::filesystem::create_directory(std::filesystem::path(tar));
		}
		for(auto t:pla){
			std::filesystem::path pa=t.name;pa=target/pa;
			std::filesystem::path pb=t.name;pb=tar/pb;
			
			std::filesystem::copy(pa,pb,std::filesystem::copy_options::overwrite_existing);
			std::filesystem::remove(pa);
		}
		target=tar;
		BackUp();
	}
	void insert(const std::string&name,const std::string&source){//source可以是相对也可以是绝对
		if(target.empty()){
			ErrPut("目标路径未初始化");
			return ;
		}
		std::filesystem::path psrc=source;
		std::filesystem::path pcur=std::filesystem::current_path();
		std::filesystem::path pnam=name;pnam=target/pnam;
		if(psrc.is_relative()){
			psrc=pcur/psrc;
		}
		if(std::filesystem::exists(pnam)){//文件已存在,询问是否要覆写
			std::string Q;
			do{
				ErrPut("文件已存在,是否要覆写(y/n)\n");
				InPut(Q);
			}while(Q.size()!=1||(Q[0]!='y'&&Q[0]!='n'));
			if(Q[0]=='n')return ;
		}
		pla.push_back(name);
		std::filesystem::copy(psrc,pnam,std::filesystem::copy_options::overwrite_existing);
		BackUp();
	}
	std::string getTarget(){
		return target.string();
	}
};

int main(){
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	
	Application::getInstance();
	while(true){
		int a;std::string A,B;
		InPut(a);
		if(a==1)InPut(A),Application::getInstance()->setTarget(A);
		else if(a==2){
			InPut(A,B);
			Application::getInstance()->insert(A,B);
		}
		else if(a==3){
			ErrPut(Application::getInstance()->getTarget(),'\n');
			Flush(std::cerr);
		}
		else return 0;
	}
	
	return 0;
}