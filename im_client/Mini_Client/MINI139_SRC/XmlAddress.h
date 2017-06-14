#include <map>
#include <vector>

// 获取所有的省
void GetAllProvince(std::map<int, std::string> &mapProvince);

// 获取所有的市
void GetAllCity(int nProvinceID, std::map<int, std::string> &mapCity);

// 获取所有的区县
void GetAllDistrict(int nProvinceID, int nCityID, std::map<int, std::string> &mapDistrict);