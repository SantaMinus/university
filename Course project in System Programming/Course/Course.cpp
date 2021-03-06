#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

struct Tree{
		std::vector<std::string> master;
		std::vector<std::string> slave;
};

void Comment(FILE *fp){
	puts("\nA comment detected");
	while(feof(fp)==0){
		char c=fgetc(fp);
		if(c=='/') 
			do 
				c = fgetc(fp); 
			while (c != '\n');
		else 
			if(c=='*'){ 
				do 
					c = fgetc(fp); 
				while (c != '*'); 
				if(fgetc(fp)=='/') 
					return;
				else{ 
					while (feof(fp)==0){ 
						c = fgetc(fp);
						if(c=='/')
							return;
					}
					puts("Incorrect comment");
				}
			}
		return;
	}
}

std::string Skip(FILE *fp){
	std::string s;
	printf("\n\nSkip:   ");
	while(feof(fp)==0){
		char c=fgetc(fp);
		printf("%c",c);
		s+=c;
		if(c=='/') 
			Comment(fp);
		if(c==' '||c=='\t'||c=='\n'||c=='/'||c=='<'||c=='>'||c=='"'||c=='*'||c==' ')
			break;
	}
	return s;
}

std::string ReadUntil(FILE *fp){
	printf("\nRead:  ");
	std::string str;
	fgetc(fp);
	int i=0;
	while(feof(fp)==0){
		char c=fgetc(fp);
		if(c==' '||c=='\t'||c=='\n'||c=='/'||c=='<'||c=='>'||c=='"'||c==' ')
			break;
		str+=c;
		printf("%c  ",str[i]);
		i++;
	}
	printf("\n%s", str.c_str());
	return str;
}



int main(int argc, char **argv){
	FILE *fp;
	std::string filename="test.c";
	std::vector<std::string> libs;
	std::vector<std::string> processed;
	std::string s, adds;
	Tree tree;
	long int filelength; 

	fp=fopen(filename.c_str(),"rt");
	if(fp==NULL){
		perror("ERROR");
		getchar();
		return 0;
	}
	puts("File opened");

	fseek(fp,0,SEEK_END);
	filelength=ftell(fp);
	printf("The file %s is of %li bytes",filename.c_str(),filelength);
	rewind(fp);

	libs.push_back("  ");

	for(int j=0;j<libs.size();j++){
		fp=fopen(filename.c_str(),"rt");
		if(fp!=NULL){
			printf("\n------%s------",filename.c_str());
			while(feof(fp)==0){
				int c=fgetc(fp);
				if(c=='/') 
					Comment(fp);
				else{
 					if(c=='#'){
						adds=Skip(fp);
						if(adds=="include "){
							s=ReadUntil(fp);
							libs.push_back(s);
							tree.master.push_back(filename);
							tree.slave.push_back(s);
						}
						adds="";
					}
				}
			}
		}

		if(std::find(processed.begin(), processed.end(), libs[j])==processed.end()&&j<libs.size()-1){
			filename=libs[j+1];
		}
	}

	if(fp!=NULL)
		fclose(fp);

	printf("\n");
	for(int i=0;i<libs.size();i++){
		printf("\n%s", libs[i].c_str());
	}

	puts("\n\nDependency tree");
	for(int i=0;i<tree.master.size();i++){
		printf("\n%s ---> %s", tree.master[i].c_str(), tree.slave[i].c_str());
	}

	getchar();
	return 0;
}
