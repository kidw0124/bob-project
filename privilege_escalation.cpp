#include <bits/stdc++.h>
using namespace std;

int main(void)
{
	// Dec 16 08:25:43 localhost root: root [22954]   852  g++ -o test8 test8.cpp --std=c++23 [0]
	system("cat /var/log/commands.log | grep / >> /root/source.txt");

	ifstream ifs("/root/source.txt");
	if(!ifs){
		cerr<<"No such file";
	}
	string tmp;
	vector<string>nonnormal;
	int tc=0;	
	ofstream ofs("/var/log/privilege.log");
	while(getline(ifs,tmp)){
		stringstream ss(tmp);
		vector<string>str(7);
		for(int i=0;i<3;i++){
			ss>>str[i];
		}
		ss>>tmp;
		ss>>str[3]>>str[4];
		ss>>tmp>>tmp;
		while(ss>>tmp){
			str[5]+=tmp+' ';
		}
		str[5].pop_back();
		size_t lastspace = str[5].find_last_of(' ');
		str[6]=str[5].substr(lastspace+1);
		str[5]=str[5].substr(0,lastspace);
		str[3].pop_back();
		for(string&k:str){
			ofs<<k<<' ';
		}
		if(str[3]!="root" && str[4]=="root"){
			ofs<<"\'privilege escalation has been tried\'\n";
			string log;
			for(string&k:str)log+=k+' ';
			nonnormal.push_back(log);
		}
		else ofs<<"\'normal execution\'\n";
	}
	ofs<<"\n\n====Privilege Escalation Tried====\n";
	ofs<<"total : "<<nonnormal.size()<<'\n';
	for(auto&k:nonnormal){
		ofs<<k<<'\n';
	}
	ifs.close();
	ofs.close();
	return 0;
}

