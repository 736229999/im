#include <map>
#include <vector>

// ��ȡ���е�ʡ
void GetAllProvince(std::map<int, std::string> &mapProvince);

// ��ȡ���е���
void GetAllCity(int nProvinceID, std::map<int, std::string> &mapCity);

// ��ȡ���е�����
void GetAllDistrict(int nProvinceID, int nCityID, std::map<int, std::string> &mapDistrict);