/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2013 t0nyren
 * Copyright (C) 2013 t0nyren
 */
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <dirent.h>
#include<sys/stat.h>
#include<sys/types.h>
#include <string>
#include "detector.h"
#include "classifier.h"

using namespace cv;
using namespace std;

int main( int argc, char** argv ) 
{
	ofstream fout;
	ofstream dbout;
	fout.open("people.csv", ofstream::out);
	dbout.open("db.dat", ofstream::out); //format: <classname> \n <filename> \n <id> <code>
	double t;
	int ms;
    
	if (argc < 2)
	{
	fprintf(stderr, "Usage: exportCode  <path_to_input_image_dir> <isDetect>\n");
	exit(1);
	}

	Detector detector;
	Classifier classifier;

	mkdir(argv[2], S_IRWXU);
	DIR *pDIR;
	struct dirent *entry;
	struct stat buf;
	pDIR = opendir(argv[1]);
	if(( pDIR=opendir(argv[1])) == NULL){
		std::cout<<"cannot open input dir"<<std::endl;
		exit(1);
	}
	entry = readdir(pDIR);
	int id = 1;
	while(entry != NULL)
	{
		if(0 != strcmp( ".", entry->d_name) && //Skip these dir
		   0 != strcmp( "..", entry->d_name) )
		{
			char * name = entry->d_name;
			stat(name, &buf);
			std::cout << name<<std::endl;
			std::string s2 = name;
			fout<<id<<','<<name<<',';
			int goodCount = 0;
			
			DIR* pDIR2;
			std::string s3 = argv[1];
			std::string origin_path = s3 + '/' + s2;
			pDIR2 = opendir(origin_path.data());
			struct dirent *entry2;
			entry2 = readdir(pDIR2);
			while(entry2 != NULL){
				if(0 != strcmp( ".", entry2->d_name) && 0 != strcmp( "..", entry2->d_name) )
				{
					//std::cout<<"\t"<<entry2->d_name<<std::endl;
					std::string img_origin_path = origin_path + '/' + entry2->d_name;
					Mat src;
					if (atoi(argv[2])==1)
						src  = detector.detect(img_origin_path.data());
					else
						src = imread(img_origin_path.data());
					Mat img;
					if (src.empty()){
						entry2 = readdir(pDIR2);
						continue;
					}
					goodCount++;
					if (src.channels() == 3 || src.channels() == 4){
						cvtColor(src, img, CV_RGB2GRAY);
					}
					else if (src.channels() == 1){
						img = src;
					}
					else{
						cout<<"channel error: "<<src.channels()<<endl;
						entry2 = readdir(pDIR2);
						continue;
					}
					float* code = classifier.encodeImg(img);
					dbout<<entry->d_name<<endl;
					dbout<<entry2->d_name<<endl;
					dbout<<id<<" ";
					for (int i = 0; i < classifier.getCodeDimension(); i++){
						dbout<<code[i]<<" ";
					}
					dbout<<endl;
					delete [] code;
					code = NULL;
				}
				entry2 = readdir(pDIR2);
			}
			fout<<goodCount<<endl;
			id++;
			closedir(pDIR2);
			
		}
		entry = readdir(pDIR);             //Next file in directory        
	}
	closedir(pDIR);
	fout.close();
}
