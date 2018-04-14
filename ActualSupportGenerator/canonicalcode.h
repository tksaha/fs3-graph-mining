/*
 *  canonicalcode.h
 *
 *  Created on: Oct 7, 2012
 *  Author: Tanay Kumar Saha (tksaha@cs.iupui.edu)
 */

#ifndef CANONICALCODE_H_
#define CANONICALCODE_H_

#include<vector>
#include "graph.h"
using namespace std;



class canonicalcodeGenerator{
	graph *g;
	std::vector<canonicalcodeStruc*> code; 
	std::vector<canonicalcodeStruc*>newlevelcode;

public:	
	canonicalcodeGenerator(graph *gx){
		code.clear();
		g=gx;
		newlevelcode.clear();
        }
	~canonicalcodeGenerator(){
		code.clear();
		newlevelcode.clear();
	};
	
	canonicalcodeStruc* getcanonicalCode(){
		getfirstlabelCanonicalCode();
		if(g->myedges.size()>1) populateNextLevels();
		return code[0];
	}
	void printCanonicalCode();
private:
	void insert (bool , int , int, string );
	bool generateAnEdge(int endvertex, canonicalcodeStruc *it);
	bool comparecodesAndInsert (canonicalcodeStruc *newcode);
	void getfirstlabelCanonicalCode();
	void populateNextLevels();
	
	int absmine(int a,int b)
	{
		if((a-b)<0) return -(a-b);
		else return (a-b);
	}
};
#endif /* CANONICALCODE_H_ */
