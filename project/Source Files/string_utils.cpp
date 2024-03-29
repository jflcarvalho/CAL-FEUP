#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cmath>
#include "string_utils.h"
using namespace std;

vector<string> splitSentence(string sentence) {
	istringstream iss(sentence);
	vector<string> tokens{ istream_iterator<string>{iss},
		istream_iterator<string>{} };

	if (tokens.size() == 0)
	{
		tokens.push_back("");
	}
	return tokens;
}

string to_lower(string &text) {
	for (size_t i = 0; i < text.size(); i++)
	{
		text[i] = tolower(text[i]);
	}
	return text;
}

void eliminateRedudantWords(vector<string> &text) {
	for (auto it = text.begin(); it != text.end(); it++)
	{
		string current = (*it);
		if (current == "Rua" || current == "Avenida" || current == "Praca" || current == "Largo" || current == "Patio" || current == "Beco" || current == "Tunel")
		{
			text.erase(it);
			it--;
		}
		else if (current == "de" || current == "do" || current == "da" || current == "dos" || current == "das")
		{
			text.erase(it);
			it--;
		}
	}
}

int aproximate_matching(string pattern, string text) {

	int totalEditDistance = 0;
	int currentEditDistance;
	int currentTotalDistance = 0;
	vector<string> textSplitted = splitSentence(text);
	vector<string> patternSplitted = splitSentence(pattern);
	if (textSplitted.size() != patternSplitted.size())
	{
		totalEditDistance += abs((int)(textSplitted.size() - patternSplitted.size()));
	}
	eliminateRedudantWords(textSplitted);
	eliminateRedudantWords(patternSplitted);
	for (vector<string>::iterator itP = patternSplitted.begin(); itP != patternSplitted.end(); itP++)
	{
		for (vector<string>::iterator itT = textSplitted.begin(); itT != textSplitted.end(); itT++)
		{
			currentEditDistance = editDistance(*itP, *itT);
			if (currentEditDistance == 0)
			{
				currentTotalDistance = currentTotalDistance / 2;
				break;
			}
			currentTotalDistance += currentEditDistance;
		}
		totalEditDistance += currentTotalDistance;
	}

	return totalEditDistance;
}


int editDistance(string pattern, string text) {
	int n = text.length();
	vector<int> d(n + 1);
	int old, neww;
	for (int j = 0; j <= n; j++)
		d[j] = j;
	int m = pattern.length();
	for (int i = 1; i <= m; i++)
	{
		old = d[0];
		d[0] = i;
		for (int j = 1; j <= n; j++)
		{
			if (pattern[i - 1] == text[j - 1]) neww = old;
			else
			{
				neww = min(old, d[j]);
				neww = min(neww, d[j - 1]);
				neww = neww + 1;
			}
			old = d[j];
			d[j] = neww;
		}
	}
	return d[n];
}

vector<Street *> aproximado(const StreetCleaned &streets, const string &nameStreet) {
	int minValue = INT_MAX;
	APR current;
	vector<APR> heap;
	vector<Street *> topToReturn;
	for (StreetCleaned::const_iterator it = streets.begin(); it != streets.end(); it++)
	{
		string temp = (*it)->getName();
		int actualValue = aproximate_matching(nameStreet, (*it)->getName());

		current.first = actualValue;
		current.second = (*it);
		heap.push_back(current);
	}
	make_heap(heap.begin(), heap.end(), APR_Greater_Than());
	heap = vector<APR>(heap.begin(), heap.begin() + 5);
	for (vector<APR>::iterator it = heap.begin(); it != heap.end(); it++)
	{
		topToReturn.push_back(it->second);
	}
	return topToReturn;
}

void pre_kmp(string pattern, vector<int> & prefix) {
	int m = pattern.length();
	prefix[0] = -1;
	int k = -1;
	for (int q = 1; q < m; q++)
	{
		while (k > -1 && pattern[k + 1] != pattern[q])
			k = prefix[k];
		if (pattern[k + 1] == pattern[q]) k = k + 1;
		prefix[q] = k;
	}
}

int kmp(string text, string pattern, const vector<int> &prefix) {
	int num = 0;
	int m = pattern.length();
	int n = text.length();
	int q = -1;
	for (int i = 0; i < n; i++)
	{
		while (q > -1 && pattern[q + 1] != text[i])
			q = prefix[q];
		if (pattern[q + 1] == text[i])
			q++;
		if (q == m - 1)
		{
			num++;
			q = prefix[q];
		}
	}
	return num;
}



vector<Street*> exata(const StreetCleaned &streets, const string &nameStreet) {
	vector<Street *> topToReturn;
	int foundMatch;
	string temp;
	temp = nameStreet;
	string userInput = to_lower(temp);
	vector<int> prefix(userInput.length());
	pre_kmp(userInput, prefix);

	for (StreetCleaned::const_iterator it = streets.begin(); it != streets.end(); it++)
	{

		foundMatch = kmp(to_lower((*it)->getName()), userInput, prefix);
		if (foundMatch)
		{
			topToReturn.push_back((*it));
		}
	}
	return topToReturn;
}
