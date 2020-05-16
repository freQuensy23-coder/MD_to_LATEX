﻿#include <fstream>
#include <iostream>
#include <regex>
#include <string>
//#include <ofstream>
using namespace std;

bool images_found = false;
bool links_found = false;


string replace_heading(string markdown) {
	//There are 6 heading's type in MD, and only 5 in Latex, so ## and ### linked to the same tag "\huge"

	regex large("######");   //large
	markdown = regex_replace(markdown, large, "\\large ");

	regex L_large("#####"); //Large
	markdown = regex_replace(markdown, L_large, "\\Large ");

	regex LL_large("####"); //LARGE
	markdown = regex_replace(markdown, LL_large, "\\LARGE ");

	regex huge("###"); //huge
	markdown = regex_replace(markdown, huge, "\\huge ");

	regex h_huge("##"); //huge
	markdown = regex_replace(markdown, h_huge, "\\huge ");

	regex H_huge("#"); //Huge
	markdown = regex_replace(markdown, H_huge, "\\Huge ");

	return markdown;
}


string replace_heading_alternative(string MD) {
	regex alt_h1("(.+)\n=+");
	regex alt_h2("(.+)\n-+");

	MD = regex_replace(MD, alt_h1, "\\Huge $1");
	MD = regex_replace(MD, alt_h2, "\\huge $1");

	return MD;
}


string replace_links(string MD) {
	//[I'm an inline-style link](https://www.google.com)
	//\href{http://www.latex-tutorial.com}{LaTeX-Tutorial}

	regex link("\\[(.*)\\]\\((.*)\\)");
	string tex = regex_replace(MD, link, "\\href{$2}{$1}");


	if (tex != MD) {
		::links_found = true;
	}

	return tex;
}


string replace_images(string MD) {
	//![alt text](https://github.com/adam-p/markdown-here/raw/master/src/common/images/icon48.png)
	//\includegraphics{https://github.com/adam-p/markdown-here/raw/master/src/common/images/icon48.png}

	regex image("!\\[.*\\]\\((.*)\\)");

	string tex = regex_replace(MD, image, "\\includegraphics{$1}");
	if (tex != MD) {
		::images_found = true;
	}

	return tex;
}


string replace_bold(string MD) {
	regex bold_text("__(.*?)__");

	MD = regex_replace(MD, bold_text, "\\textbf{$1}");

	return MD;
}


string replace_bold_alternative(string MD) {
	//__Bold text. Spaces are avalible. "_" too, but only single__
	//\textbf{Bold text. Spaces are avalible. "_" too, but single}

	regex bold_text("\\*\\*[^\\*{1}](.*?)\\*\\*");

	MD = regex_replace(MD, bold_text, "\\textbf{$1}");

	return MD;
}


string replace_italic(string MD) {
	regex italic_text("_(.*?)_");
	MD = regex_replace(MD, italic_text, "\\textit{$1}");
	return MD;
}


string replace_italic_alternative(string MD) {
	regex bold_text("\\*(.*?)\\*");

	MD = regex_replace(MD, bold_text, "\\textit{$1}");

	return MD;
}


string module_manager() {
	string modules = "";
	if (::images_found) {
		modules = modules + "\\usepackage{graphicx}" + "\n";
	}
	if (::links_found) {
		modules = modules + "\\usepackage{hyperref}" + "\n";
	}
	return modules;
}


string convert(string MD)
{
	MD = replace_heading(MD);
	MD = replace_heading_alternative(MD);
	MD = replace_images(MD);
	MD = replace_links(MD);
	MD = replace_bold(MD);
	MD = replace_bold_alternative(MD);
	MD = replace_italic(MD);
	MD = replace_italic_alternative(MD);
	MD = "\\begin{document} \n" + MD;        //preamble
	MD = module_manager() + MD;
	MD = "\\documentclass{article} \n " + MD + "\n\\end{document}";
	return MD;
}


string file_read(string file_name)
{
	string result = "";
	try
	{
		ifstream inf(file_name);
		while (inf)
		{
			string strInput;
			getline(inf, strInput);
			result = result + strInput + "\n";
		}
		return result;
	}

	catch (const exception& ex) {
		return "";
	}
}


void file_write(string file_name, string text) {
	try
	{
		ofstream outf(file_name);
		outf << text;
	}

	catch (const std::exception&)
	{
		cout << "Error while saving file";
	}
}

int main(int argc, char* argv[])
{
	//Getting file name
	if (true) {
		string file_input = argv[1];
		string file_out = "output.tex";
		cout << "Opening " << argv[1] << "\n";
		string MD = file_read(file_input);


		//Saving and Converting 
		if (MD != "") {
			string Tex = convert(MD);
			file_write(file_out, Tex);
			cout << "Saved as " << file_out;
		}
		else {
			cout << "Error while reading file. It may be empty or not exist at all";
		}
	}
}