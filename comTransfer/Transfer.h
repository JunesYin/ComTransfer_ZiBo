#pragma once


#include <string>

using std::string;


const CString transfer(const CString &val, const int &userID, const CString &userName, 
	char options[4], const CString &prefix, const CString &suffix);

const BOOL transfer(const CString &val, const int &userID, const CString &userName, 
	char options[4], const CString &prefix, const CString &suffix,
	const char rightAnwser, CString &data, char &anwser);