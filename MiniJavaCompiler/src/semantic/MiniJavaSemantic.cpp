#include "semantic/MiniJavaSemantic.h"

#include <iostream>
#include <vector>
#include <unordered_map>

#include "semantic/visitor/TransformToAST.h"
#include "ast/node/ASTProgram.h"

using namespace std;

void MiniJavaSemantic::analyze(MiniJavaParser::ProgContext* progCtx)
{
	/*TransformToAST transformAST;
	ASTProgram* program = transformAST.visitProgram(progCtx);

	ASTMainClass* mainClass = program->getMainClass();

	cout << "MAIN CLASS >" << endl;
	cout << "    ID: " <<  mainClass->getMainMethod()->getId() << " | RETURN: " << typeToString(mainClass->getMainMethod()->getReturnType()->getMethodType()) << endl << endl;

	cout << "CLASS >" << endl;
	std::vector<ASTClass*>* classList = program->getClassList();

	for (int i = 0; i < classList->size(); i++)
	{
		ASTClass* classDecl = classList->at(i);

		cout << "    VAR LIST >" << endl;
		std::vector<ASTVar*>* varList = classDecl->getVarList();

		for (int j = 0; j < varList->size(); j++)
		{
			ASTVar* var = varList->at(j);
			cout << "        ID: " << var->getId() << " | TYPE: " << typeToString(var->getVarType()->getVarType()) << endl;
		}
		
		cout << "    METHOD LIST >" << endl;
		std::vector<ASTMethod*>* methodList = classDecl->getMethodList();

		for (int j = 0; j < methodList->size(); j++)
		{
			ASTMethod* method = methodList->at(j);
			cout << "        ID: " << method->getId() << " | RETURN: " << typeToString(method->getReturnType()->getMethodType()) << endl;
		}
	}*/




	CollectInformation collectInformation(&errorListener);
	antlrcpp::Any result = collectInformation.visit(progCtx);
	ProgramInfo *programInfo = result.as<ProgramInfo*>();

	ExpressionChecker expressionChecker(programInfo, &errorListener);
	expressionChecker.visit(progCtx);

	unordered_map<string, ClassInfo*> classList = programInfo->getClassList();

	for (pair<string, ClassInfo*> classInfo : classList)
	{
		cout << "CLASS: " << classInfo.first << " | INHERITED: " << classInfo.second->inheritedClassId << endl;

		unordered_map<string, VarInfo*> varList = classInfo.second->varInfoMap;

		cout << "     Variable list:" << endl;

		for (pair<string, VarInfo*> varInfo : varList)
		{
			cout << "          ID: " << varInfo.second->id << " | TYPE: " << varInfo.second->type << endl;
		}

		unordered_map<string, MethodInfo*> methodList = classInfo.second->methodInfoMap;

		cout << endl << "     Method list:" << endl;

		for (pair<string, MethodInfo*> methodInfo : methodList)
		{
			cout << "          ID: " << methodInfo.second->id 
				<< " | RETURN: " << methodInfo.second->returnType 
				<< " | PARAMS: " << methodInfo.second->paramInfoMap.size()
				<< " | VARS: " << methodInfo.second->varInfoMap.size() << endl;

			unordered_map<string, VarInfo*> paramList = methodInfo.second->paramInfoMap;

			cout << "               Parameter list:" << endl;

			for (pair<string, VarInfo*> paramInfo : paramList)
			{
				cout << "                    ID: " << paramInfo.second->id << " | TYPE: " << paramInfo.second->type << endl;
			}

			unordered_map<string, VarInfo*> varList = methodInfo.second->varInfoMap;

			cout << "               Variable list:" << endl;

			for (pair<string, VarInfo*> varInfo : varList)
			{
				cout << "                    ID: " << varInfo.second->id << " | TYPE: " << varInfo.second->type << endl;
			}
		}

		cout << endl;
	}
}

bool MiniJavaSemantic::hasErrors()
{
	return errorListener.hasErrors();
}

void MiniJavaSemantic::printErrorList()
{
	errorListener.printErrorList();
}