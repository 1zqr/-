#include <io.h>
#include <string>
#include <bits/stdc++.h>
using namespace std;

clock_t startTime,stopTime;
double totalTime = 0.0;
/*string title = "\n词频统计：\n";
string helpInformation = title +
                         "\t功能0：输出英文文本中26字母出现的频率，由高到低排序。命令格式：wf.exe -c <file name>\n" +
                         "\t功能1：输出文件中所有不重复的单词。命令格式：wf.exe -f <file name>\n" +
                         "\t功能2：指定文件目录，对目录下每一个文件执行\" -f \"。命令格式：wf.exe -d <file name>\n" +
                         "\t       同上，但是遍历递归目录下的所有子目录。命令格式：wf.exe -d -s <file name>\n" +
                         "\t功能3：支持-n参数，输出出现次数最多的前N个单词。命令格式：wf.exe -n 10 <file name>\n" +
                         "\t       当没有指明数量时，默认列出所有单词的频率。\n" +
                         "\t功能4：支持新的命令行，支持stop words。命令格式：wf.exe -x <stop word file name> -f <file name>\n"
                         "\t如果文件名包含空格, 请使用此格式:wf.exe -c \"file_name\"\n\n";
*/

//统计字母的频率
int letterNumber[26];
int total = 0;//约3e6个字符
int len = 0;//行字符个数
char buffer[1024+1];//buffer缓冲区
struct alphabeticStruct
{
	char name;
	float letterNumber;
	alphabeticStruct(char n = 'a', float lN = 0)
	{
		name = n;
		letterNumber = lN;
	}
};
alphabeticStruct letterStruct[26];
bool isUpperLetter(char x)
{
	return 'A' <= x && x <= 'Z';
}
bool isLowerLetter(char x)
{
	return 'a' <= x && x <= 'z';
}
bool isLetter(char x)
{
	return isUpperLetter(x) || isLowerLetter(x);
}
int getLetterId(char x)
{
	if(isUpperLetter(x))
		return x - 'A';
	else
		return x - 'a';
}
void judgeLetter(char str[])
{
	for(int i = 0; str[i]; i++)
		if(isLetter(str[i]) )
		{
			total++;
			letterNumber[getLetterId(str[i])]++;
		}
}
bool cmpLetter(alphabeticStruct a, alphabeticStruct b)
{
	return (a.letterNumber > b.letterNumber) || (a.letterNumber == b.letterNumber && a.name < b.name);
}
void alphabeticStatistics(char *fileName)
{
	memset(letterNumber, 0, sizeof(letterNumber) * 26);
	total = 0;
	ifstream inFile;
	inFile.open(fileName);
	ofstream outFile;
	outFile.open(strcat(fileName, " result-c.txt"), ios::trunc|ios::out);

	if( !inFile.is_open())
	{
		cout << ":文件打开失败" << endl;
		exit(1);
	}
	while( !inFile.eof())
	{
		inFile.getline(buffer, 1024);
		len = strlen(buffer);
		judgeLetter(buffer);
	}
	inFile.close();
	for(int i = 0; i < 26; i++)
	{
		letterStruct[i].name = i +'a';
		letterStruct[i].letterNumber = 1.0 * letterNumber[i] / total * 100;
	}
	sort(letterStruct, letterStruct+26, cmpLetter);
	for(int i = 0; i < 26; i++)
	{
//		printf("%2d %c: %.2f\n", i+1, letterStruct[i].name, letterStruct[i].letterNumber);
		outFile << setw(2) << i+1 << " " << setw(4) << letterStruct[i].name << ": " << setprecision(2) << setw(8) << letterStruct[i].letterNumber << "%" << endl;
	}
	outFile.close();
}

//统计不重复单词出现的次数
void wordStatistics(char *fileName)
{
	ifstream inFile;
	inFile.open(fileName);
	char *outFileName = (char *)" result-f.txt";
	ofstream outFile;
	outFile.open(strcat(fileName, outFileName), ios::trunc|ios::out);

	if( !inFile.is_open())
	{
		cout << fileName << ":文件打开失败" << endl;
		exit(1);
	}
	string str;
	map<string,int>wordcount;

	while( !inFile.eof())
	{
		inFile >> str;
		wordcount[str]++;
	}
//	cout << fileName << "\t单词种类："<< wordcount.size() << endl;
	//优先队列使用小顶堆，排在前面的数量少，使用">";
	priority_queue<pair<int,string>, vector<pair<int,string> >, greater<pair<int,string> > > queueK;
	for (map<string,int>::iterator iter=wordcount.begin(); iter!=wordcount.end(); iter++)
		queueK.push(make_pair(iter->second,iter->first));
	pair<int,string>tmp;
	while (!queueK.empty())
	{
		tmp=queueK.top();
		queueK.pop();
//		cout<<tmp.second<<"\t"<<tmp.first<<endl;
		outFile << tmp.second << "\t\t" << tmp.first << endl;
	}
}

//遍历指定文件目录，对目录下每个文件执行wf.exe -f <file>
vector<string> getFileList(string dir)
{
	vector<string> allPath;
	//在目录后边加上"\\*.*"进行第一次搜索
	string dir2 = dir + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir2.c_str(), &findData);
	if (handle == -1)//检查是否成功
	{
		cout << "Can't find file in this Path!" << endl;
		return allPath;
	}
	do
	{
		if (findData.attrib & _A_SUBDIR)//是否含有子目录
		{
			//若该目录是"."或".."，则进行下一次循环，否则输出子目录名，并进入下一次搜索
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			//在目录后边添加"\\"和搜索到的目录名进行下一次搜索
//			string dirNew = dir + "\\" + findData.name;
//			vector<string> tempPath = getFileList(dirNew);
//			allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());
		}
		else//不是子目录，即是文件
		{
			string filePath = dir + "\\" + findData.name;
			allPath.push_back(filePath);
		}
	}
	while (_findnext(handle, &findData) == 0);
	_findclose(handle);//关闭搜索句柄

//	for(size_t i = 0; i < allPath.size(); i++)
//	{
//		string perPath = allPath.at(i);
//		cout << perPath << endl;
////		wordStatistics((char *)perPath.c_str());
//	}
	return allPath;
}

//遍历指定文件目录，但是会遍历目录下的所有子目录，对目录下每个文件执行wf.exe -f <file>
vector<string> getFileListInMenu(string dir)
{
	vector<string> allPath;
	//在目录后边加上"\\*.*"进行第一次搜索
	string dir2 = dir + "\\*.*";

	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir2.c_str(), &findData);
	if (handle == -1)//检查是否成功
	{
		cout << "Can't find file in this Path!" << endl;
		return allPath;
	}
	do
	{
		if (findData.attrib & _A_SUBDIR)//是否含有子目录
		{
			//若该目录是"."或".."，则进行下一次循环，否则输出子目录名，并进入下一次搜索
			if (strcmp(findData.name, ".") == 0 || strcmp(findData.name, "..") == 0)
				continue;
			//在目录后边添加"\\"和搜索到的目录名进行下一次搜索
			string dirNew = dir + "\\" + findData.name;
			vector<string> tempPath = getFileList(dirNew);
			allPath.insert(allPath.end(), tempPath.begin(), tempPath.end());
		}
		else//不是子目录，即是文件
		{
			string filePath = dir + "\\" + findData.name;
			allPath.push_back(filePath);
		}
	}
	while (_findnext(handle, &findData) == 0);
	_findclose(handle);//关闭搜索句柄

//	for(size_t i = 0; i < allPath.size(); i++)
//	{
//		string perPath = allPath.at(i);
//		cout << perPath << endl;
////		wordStatistics((char *)perPath.c_str());
//	}
	return allPath;
}

//支持 -n 参数，输出出现次数最多的前 n 个单词。当没有指明数量时，列出所有单词
void wordStatisticsTopK(char *fileName, const int &k)
{
	ifstream inFile;
	inFile.open(fileName);
	char *outFileName = (char *)" result-f-n.txt";
	ofstream outFile;
	outFile.open(strcat(fileName, outFileName), ios::trunc|ios::out);

	if( !inFile.is_open())
	{
		cout << fileName << ":文件打开失败" << endl;
		exit(1);
	}
	string str;
	map<string,int>wordcount;

	while( !inFile.eof())
	{
		inFile >> str;
		wordcount[str]++;
	}
	cout << fileName << "\t单词种类："<< wordcount.size() << endl;
	//优先队列使用小顶堆，排在前面的数量少，使用">";
	priority_queue<pair<int,string>, vector<pair<int,string> >, greater<pair<int,string> > > queueK;
	for (map<string,int>::iterator iter=wordcount.begin(); iter!=wordcount.end(); iter++)
	{
		queueK.push(make_pair(iter->second,iter->first));
		if(queueK.size() > k)
			queueK.pop();
	}

	pair<int,string>tmp;
	while (!queueK.empty())
	{
		tmp=queueK.top();
		queueK.pop();
//		cout<<tmp.second<<"\t"<<tmp.first<<endl;
		outFile << tmp.second << "\t\t" << tmp.first << endl;
	}
}

//停词表stop word
void erasePunct(string &s)
{
	string::iterator it = s.begin();
	while(it != s.end())
	{
		if(ispunct(*it))
			it = s.erase(it);
		else
			++it;
	}
}
void stringToLower(string &s)
{
	for(string::iterator it = s.begin();
	        it != s.end();
	        ++it)
	{
		if(isupper(*it))
			*it = tolower(*it);
	}
}

bool isAllDigit(const std::string &s)
{
	for(string::const_iterator it = s.begin();
	        it != s.end();
	        ++it)
	{
		if(!isdigit(*it))
			return false;
	}
	return true;
}


class WordFrequency
{
	public:
		WordFrequency(const std::string &filename, const std::string &stopFile);//初始化
		void ReadStopFile();
		void ReadWordFile();
		void sortWordByFrequency();
		void printWordFrequency()const;
	private:
		void addWordToDict(const std::string &word);
		bool isStopWord(const std::string &word)const;

		typedef std::vector<std::pair<std::string, int> >::iterator Wordit;//为了方便
		typedef std::vector<std::pair<std::string, int> >::const_iterator Wordkit;

		std::string filename_;
		std::string stopFile_;

		std::vector<std::string> stopList_;
		std::vector<std::pair<std::string, int> > words_;
};


WordFrequency::WordFrequency(const std::string &filename, const std::string &stopFile)
	:filename_(filename),stopFile_(stopFile)
{ }

void WordFrequency::ReadStopFile()
{
	ifstream in(stopFile_.c_str());
	if( !in )
		throw runtime_error("文件打开失败");
	string word;
	while(in >> word)
	{
		stopList_.push_back(word);
	}
	in.close();
}

void WordFrequency::ReadWordFile()
{
	ifstream infile(filename_.c_str());
	if( !infile )
		throw runtime_error("文件打开失败");
	string word;
	while(infile >> word)
	{
		erasePunct(word);
		if( isAllDigit(word))
			continue;
		stringToLower(word);
		if( !isStopWord(word))
			addWordToDict(word);
	}
	infile.close();
}

bool WordFrequency::isStopWord(const string &word)const
{
	vector<string>::const_iterator it = stopList_.begin();
	while( it  != stopList_.end())
	{
		if( *it == word)
			break;
		it ++;
	}
	return (it != stopList_.end());
}

void WordFrequency::addWordToDict(const string &word)
{
	Wordit it = words_.begin();
	while( it != words_.end())
	{
		if( it->first == word)
		{
			++ it->second ;
			break;
		}
		it ++;
	}
	if(it == words_.end())
		words_.push_back(make_pair(word, 1)) ;
}

bool cmp(const pair<string, int> &a, const pair<string, int>&b)
{
	return a.second > b.second;
}

void WordFrequency::sortWordByFrequency()
{
	sort(words_.begin(), words_.end(), cmp);
}

void WordFrequency::printWordFrequency()const
{
	string outFileName = filename_ + " result-x stop -f.txt";
	ofstream outFile;
	outFile.open(outFileName.c_str(), ios::trunc|ios::out);
	Wordkit it = words_.begin();
	while(it != words_.end())
	{
		outFile << it->first.c_str() << "\t" << it->second << endl;
//		printf("words: %s, frequency: %d\n",it->first.c_str(),it->second);
		it ++;
	}
}



//主函数
int main(int argc, char *argv[])
{
//	cout << helpInformation << endl;

	cout << "用户输入：\n";
	cout << "argc = " << argc << endl;
	for(int i = 0; i < argc; i++)
		printf(" argv[%d]: %s", i, argv[i]);
	printf("\n");

	char currentFilePath[256];
	getcwd(currentFilePath, sizeof(currentFilePath));
	cout << "当前工作路径是： " << currentFilePath << endl << endl;


	if(argc == 1)
	{
		cout << "请在命令行中运行该程序" << endl;
		return 0;
	}
	else if(argc == 2)
	{
		return 0;
	}
	else if(argc == 3)
	{
		//统计字母的频率
		if(strcmp(argv[1],"-c") == 0)
		{
			startTime = clock();

			alphabeticStatistics(argv[2]);

			stopTime = clock();
			totalTime = (double)(stopTime - startTime) / CLK_TCK;
			cout << "The total time is: " << totalTime << "s" << endl;
		}
		//统计不重复单词出现的次数
		else if(strcmp(argv[1], "-f") == 0)
		{
			startTime = clock();

			wordStatistics(argv[2]);

			stopTime = clock();
			totalTime = (double)(stopTime - startTime) / CLK_TCK;
			cout << "The total time is: " << totalTime << "s" << endl;
		}
		//遍历文件目录，对目录下每个文件执行wf.exe -f <file>
		else if(strcmp(argv[1], "-d") == 0)
		{
			startTime = clock();

			vector<string>allPath = getFileList(strcat(currentFilePath, argv[2]));
			for(size_t i = 0; i < allPath.size(); i++)
			{
				string perPath = allPath.at(i);
				cout << perPath << endl;
				cout << "\t\t" << (char *)perPath.c_str() << endl;
				wordStatistics( (char *)perPath.c_str() );
			}

//			getFileList(strcat(currentFilePath, argv[2]));

			stopTime = clock();
			totalTime = (double)(stopTime - startTime) / CLK_TCK;
			cout << "The total time is: " << totalTime << "s" << endl;
		}
		//支持 -n 参数，输出出现次数最多的前 n 个单词。当没有指明数量时，列出所有单词
		else if(strcmp(argv[1], "-n") == 0)
		{
			startTime = clock();

			wordStatistics(argv[2]);

			stopTime = clock();
			totalTime = (double)(stopTime - startTime) / CLK_TCK;
			cout << "The total time is: " << totalTime << "s" << endl;
		}
//		else
//			cout << helpInformation << endl;
	}
	else if(argc == 4)
	{
		//历指定文件目录，但是会遍历目录下的所有子目录，对目录下每个文件执行wf.exe -f <file>
		if(strcmp(argv[1], "-d") == 0 || strcmp(argv[2], "-s") == 0)
		{
			startTime = clock();

			vector<string>allPath = getFileListInMenu(strcat(currentFilePath, argv[3]));
			for(size_t i = 0; i < allPath.size(); i++)
			{
				string perPath = allPath.at(i);
				cout << perPath << endl;
				wordStatistics((char *)perPath.c_str());
			}

//			getFileListInMenu(strcat(currentFilePath, argv[3]));

			stopTime = clock();
			totalTime = (double)(stopTime - startTime) / CLK_TCK;
			cout << "The total time is: " << totalTime << "s" << endl;
		}
		//支持 -n 参数，输出出现次数最多的前 n 个单词。
		else if(strcmp(argv[1], "-n") == 0 || atoi(argv[2])%1 == 0)
		{
			startTime = clock();

			wordStatisticsTopK(argv[3], atoi(argv[2]));

			stopTime = clock();
			totalTime = (double)(stopTime - startTime) / CLK_TCK;
			cout << "The total time is: " << totalTime << "s" << endl;
		}
	}
	else if(argc == 5)
	{
		//停词表stop world
		if(strcmp(argv[1], "-x") == 0 || strcmp(argv[3], "-f") == 0)
		{
			startTime = clock();

//			WordFrequency wf(argv[4], argv[2]);
//			wf.ReadStopFile();
//			wf.ReadWordFile();
//			wf.sortWordByFrequency();
//			wf.printWordFrequency();

			stopTime = clock();
			totalTime = (long)(stopTime - startTime) / CLK_TCK;
			cout << "The total time is: " << totalTime << "s" << endl;
		}
	}
	return 0;
}
