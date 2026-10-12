#include<iostream>
#include<ctime>//获取时间头文件
#include <random>//随机数头文件
using namespace std;
int main()
{
	//第一步,获取种子
	//std::mt19937 是随机数种子，有3种常见的获取方法

	//1,固定种子,每次生成的序列相同
	std::mt19937 reg_gen(13331);

	//2,简单时间种子,精确到秒
	std::mt19937 time_gen(static_cast<unsigned int>(std::time(nullptr)));

	//3,操作系统获取熵种子,从操作系统获取熵后处理出来的种子(最常用)
	std::random_device rd;
	std::mt19937 gen(rd());

	/*----------------------------------------------------------------------*/

	//第2步,定义分布对象(默认熵种子)

	//1,整形,均匀分布   范围; [0,100]
	std::uniform_int_distribution<int> number_dist(0, 100);
	
	//2,浮点型,均匀分布   范围: [0.0,1.0)
	std::uniform_real_distribution<double> float_dist(0.0, 1.0);

	//3,浮点型,正态分布   均值:10.0 , 标准差:2.0
	std::normal_distribution<double> nd_dist(10.0, 2.0);

	/*----------------------------------------------------------------------*/

	//第三步,生成随机数
	//格式: 分布对象(随机种子);
	
	//例子:   生成10个 整形,均匀分布   范围; [0,100] 的随机数
	for (int i = 0; i < 10; ++i) cout << number_dist(gen) << ' ';

	return 0;
}
