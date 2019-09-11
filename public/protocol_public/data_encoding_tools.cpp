#include "data_encoding_tools.hpp"
#include "algorithm.hpp"
/*
����������ô�������ٶȳ���ƥ�����ĵ�����������������ı��ʽ�����linux������3�ֽڵ�UTF-8Ҳ�����������ƥ����Ȼ�ǲ���ȷ�ġ�
Linux�����µ�����ƥ�������

\un�����е� n?��һ�����ĸ�ʮ���������ֱ�ʾ�� Unicode�ַ�������֧�������ֽڵ�utf-8

\xn������?n?Ϊʮ������ת��ֵ��ʮ������ת��ֵ����Ϊȷ�����������ֳ������磬 '\x41'ƥ�� "A"��'\x041' ��ȼ��� '\x04' & "1"��������ʽ�п���ʹ�� ASCII���롣

��˵ÿ��ǽ������ֽ�ת���2���ֽڣ��ٽ������ĵ�ƥ�䡣

�ȿ�˫�ֽڵ�unicode�����ת�������ֽڵ�utf-8

|  Unicode���ŷ�Χ      |  UTF-8���뷽ʽ
n |  (ʮ������)           | (������)
---+-----------------------+------------------------------------------------------
1 | 0000 0000 - 0000 007F |                                              0xxxxxxx
2 | 0000 0080 - 0000 07FF |                                     110xxxxx 10xxxxxx
3 | 0000 0800 - 0000 FFFF |                            1110xxxx 10xxxxxx 10xxxxxx
4 | 0001 0000 - 0010 FFFF |                   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
5 | 0020 0000 - 03FF FFFF |          111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
6 | 0400 0000 - 7FFF FFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
����֪��

\u4e00-\u9fa5
����n=3�ı��봦������4E00����ô������ת����16����(����16λ����ǰ�油0)

0000 1110 0000 0000
���ݵ����еĸ�ʽ��Ҳ���ǽ�����16������λ���¶Ͼ�
0000 111000 000000
�������ҷֱ���õ����µ�xx��
1110xxxx 10xxxxxx 10xxxxxx
�õ����ֽڵ�UTF-8����
11100000 10111000 10000000
Ҳ����0xE0 B8 80
�������֪���޷�ֻ��һ����Χ��ȷ�����ֽڵ�����Ҫ��ô��ʾ�����Ի��ǽ���ת����˫�ֽ���ƥ�����ġ�

�����������ƣ�����֪�����ֽڵ�UTF-8���ת���˫�ֽڵ�unicode���롣��Ȼ֪��ԭ����û�б�Ҫ�����������ж��ַ��Ƿ�Ϊ�����ˡ�



�ο�:
https://stackoverflow.com/questions/40054732/c-iterate-utf-8-string-with-mixed-length-of-characters

*/

#if 0
void linux_split_utf8_string(char *fulline) 
{
	int sum = strlen(fulline);
	std::string lines;
	for(int cur = 0;cur<sum;cur++)
	{		
		char t = fulline[cur];
		if((t&0xE0) == 0xE0)
		{	//3byte
			lines = lines + t + fulline[cur+1] + fulline[cur+2];
			cur += 3;

		}
		else if((t&0xC0) == 0xC0)
		{//2byte
			lines = lines + t + fulline[cur+1];
			cur += 2;
		}
		else
		{//1byte
			lines = lines + t;
			cur++;
		}
		OutputDebugString(s2ws(lines).c_str());
	}
}

#endif

void windows_split_utf8_string(const std::string& s, std::vector<std::string>& strings)
{
	for (size_t i = 0; i < s.length();)
	{
		int cplen = 1;
		if ((s[i] & 0xf8) == 0xf0)      // 11111000, 11110000
		{
			cplen = 4;
		}
		else if ((s[i] & 0xf0) == 0xe0) // 11100000
		{
			cplen = 3;
		}
		else if ((s[i] & 0xe0) == 0xc0) // 11000000
		{
			cplen = 2;
		}
		if ((i + cplen) > s.length())
		{
			cplen = 1;
		}
		strings.push_back(s.substr(i, cplen));
		i += cplen;
	}
}

std::string append_header_to_string(const std::string& content, const std::string& number)
{
	size_t n = 4; //ȡ���n������
#ifdef WIN32
	const std::wstring wh1 = L"β��Ϊ";
	const std::wstring wh2 = L"�Ŀͻ�:";
	std::string h1 = ws2s(wh1);
	std::string h2 = ws2s(wh2);
#else
	const std::string h1 = "β��";
	const std::string h2 = "�ͻ�:";
#endif
	std::string h3;
	if(number.length() <= n) 
	{
		h3 = number;
	}
	else
	{
		h3 = number.substr(number.length() - 4, 4);
	}
	std::string s = h1 + h3 + h2 + content;
	return s;
}


std::string insert_char_to_string(const std::string& content , int index)
{
	const int CHAR_COUNT = 80;
	const char chars[CHAR_COUNT] = {',', '.', ';', '#', '$', '^', '*', ' ', '~', '(', ')', '[', ']', '&', ':', '<', '>', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8'
	, '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F'
	, 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
	std::vector<std::string> content_vec;
	windows_split_utf8_string(content, content_vec);
	int len = 0; //ֻ�����ĺ�����룬���ֽڵ��ַ�(Ӣ�ģ����ֵ�)������
	for(int i = 0; i < content_vec.size(); i++)
	{
		if(content_vec.at(i).length() > 1)
		{
			len++;
		}
	}
	//ת�˵ڼ�Ȧ��
	int round = index / len;
	int pos = index % len;
	char c = chars[round % CHAR_COUNT];
	std::string new_content;

	int total_length = content_vec.size();
	bool already_insert = false;
	for(size_t i = 0; i < total_length; i++)
	{
		new_content += content_vec.at(i);
		if(i >= pos && content_vec.at(i).length() > 1 && !already_insert) //���ֽں��治����
		{
			new_content += c;
			already_insert = true;
		}
	}
	return new_content;
}

#ifdef WIN32
void data_encoding_tools_test()
{
	std::vector<std::string> strings;
	std::string content = ws2s(L"��a����b����.");
	windows_split_utf8_string(content, strings);
	for(std::vector<std::string>::iterator it = strings.begin(); it != strings.end(); it++) {
		OutputDebugString(s2ws(*it).c_str());OutputDebugString(L"\n");
	}
	
	OutputDebugString(s2ws(append_header_to_string(content, "18665152605")).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 0)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 1)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 2)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 3)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 4)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 5)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 6)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 7)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 8)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 9)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 10)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 100)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 1000)).c_str());OutputDebugString(L"\n");
	OutputDebugString(s2ws(insert_char_to_string(content, 10000)).c_str());OutputDebugString(L"\n");
}
#endif