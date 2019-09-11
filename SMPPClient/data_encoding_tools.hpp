#ifndef __DATA_ENCODING_H__
#define __DATA_ENCODING_H__

#include <vector>
//void linux_split_utf8_string(char *fulline);
void windows_split_utf8_string(const std::string& s, std::vector<std::string>& strings);

//�������������:��ӭ���ٰ�����˹ɽ��������18665152605��������ת��Ϊ���װ���2605�û�����ӭ���ٰ�����˹ɽ
std::string append_header_to_string(const std::string& content, const std::string& number);

//�������������:��ӭ���ٰ�����˹ɽ
//indexΪ0������ת��Ϊ����.ӭ���ٰ�����˹ɽ
//indexΪ0������ת��Ϊ����ӭ.���ٰ�����˹ɽ
//indexΪ10������ת��Ϊ����,ӭ���ٰ�����˹ɽ
std::string insert_char_to_string(const std::string& content, int index);

#endif