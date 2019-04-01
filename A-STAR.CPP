#include<iostream>
#include<cstdlib>
#include<vector>
#include<unordered_map>
#include<string>
#include<cmath>
using namespace std;

//test A-star
/*
**A-star算法实战
**2018年2月13日10:55:07
**追梦少年  QQ：1131052403
**开源的狂热爱好者，代码风骚，效率恐怖
*/

//坐标
class Point
{
public:
	int x;
	int y;

	Point() = default;
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

//代表一个数据
class Data
{
public:
	Point point;
	double g;//走到当前点需要的代价
	double h; //从当前点走到末尾需要有的代价
	Data * parent;//存储父节点的指针

	//构造初始化
	Data(Point point, double g, double h, Data * parent)
	{
		this->g = g;
		this->h = h;
		this->parent = parent;
		this->point = point;
	}
	//求总代价
	double f()
	{
		return g + h;
	}
};


//构建最小二叉堆
class minHeap
{
public:
	//vector构建最小大根堆，不要存对象，存进来的对象是副本。
	vector<Data *>  vector;
	//hashMap是一个开启列表，所有能够走的点
	unordered_map<string,Data> hashMap;
	//总个数
	int endPnt = 0;;
	
	//得到并且移除顶部最小的节点。
	Data *  getAndRemoveMin()
	{
		//不为空取值，为空返回null
		if (!isEmpty())
		{
			Data * head = vector[0];//取第一个备份
			Data * end = vector[endPnt - 1];//取最后一个
			vector[0] = end;//最后一个移动到第一个
			endPnt--; //个数减1
			hashMap.erase(getKey(head->point));//开启列表中删除
			TopDown(); //重新调整二叉堆
			return head;//返回最小
		}
		else
		{
			 return nullptr;
		}
	}

	//判断是否为空
	bool isEmpty()
	{
		return endPnt<=0;
	}

	//添加元素到二叉堆
	void add(Data * data)
	{
		//如果vector的大小大于endPnt，必须在endPnt位置开始添加数据。因为有垃圾值
		if (vector.size()>endPnt)
		{
			vector[endPnt] =data;
		}
		else
		{
			vector.push_back(data);
		}
		//个数加1
		endPnt++;
		//添加进开启列表
		hashMap.insert(pair<string, Data>(getKey(data->point),*data));
		bottomUp();
	}

	//根据坐标查找开启列表
	Data * find(Point point)
	{
		if (hashMap.find(getKey(point)) != hashMap.end())
		{
				return	&(hashMap.find(getKey(point))->second);
		}
		return nullptr;
	}


	//根据坐标构建开启列表Map中key值
	string getKey(Point point)
	{
		char str[7]{ 0 };
		sprintf_s(str, "%d-%d", point.x, point.y);
		string key(str);
		return key;
	}


	//重新调整二叉堆。只有第一个元素改变了，其他元素仍然保留了二叉堆的性质。
	//从第一个元素开始调整
	void TopDown()
	{
		for (int cur = 0; cur < endPnt;)
		{
			//左节点位置
			int left = 2 * cur + 1;
			//右节点位置
			int right = 2 * cur + 2;
			//当前节点
			Data * dc = vector[cur];

			//next存储最小元素的位置
			int next = -1;
			//dn代表最小的那个节点，此刻为当前节点
			Data * dn = dc;
			/*
			**找到最小的
			*/
			//dn最小节点与左节点对比代价值
			if (left < endPnt && vector[left]->f() < dn->f())
			{
				next = left;
				dn = vector[left];
			}
			//dn最小节点与右节点对比代价值
			if (right < endPnt && vector[right]->f() < dn->f())
			{
				next = right;
				dn = vector[right];
			}
			//到此next存储了最小值，dn存储了最小元素指针
			if (next >= 0 && next < endPnt)
			{
				vector[cur] = dn;
				vector[next] = dc;
				cur = next;
			}
			else  //一直进行下去直到元素比其值元素都大，或是到达了末尾。
			{
				break;
			}
		}
	}

	//重新调整二叉堆。
	//二叉堆添加元素的时候调用。每一次添加元素在最后。其他的元素已经是最小二叉堆了。
	
void bottomUp()
{
	//从最后这个元素开始调整
	for (int cur = endPnt - 1; cur >= 0;)
	{
		//父节点
		int parent = (cur - 1) / 2;
		//没有父节点
		if (parent < 0)
		{
			break;
		}
		//当前节点
		Data * dc = vector[cur];

		//子节点小于父节点，交换
		if (parent >= 0 && vector[cur]->f() < vector[parent]->f())
		{
			vector[cur] = vector[parent];
			vector[parent] = dc;
			cur = parent;
		}
		else//父节点不存在或者父节点更小，直接退出。
		{
			break;
		}
		}
	}
};



const  char START = 'S';  // 起点
const  char END = 'E';  // 终点
const  char SPACE = '.';  // 空地
const char WALL = 'W';  // 墙
const  char VISITED = '-';  // 被访问过
const char ON_PATH = '@';  // 在结果路径上
string str;

// 地图字符串，用于初始化地图
static  string S_MAP[10]{
	". . . . . . . . . . . . . . . . . . . .",
	". . . W W W W . . . . . . . . . . . . .",
	". . . . . . W . . . . . . . . . . . . .",
	". . . . . . W . . . . . . . . . . . . .",
	". . S . . . W . . . . . . . . . . . . .",
	". . . . . . W . . . . . . . . . . . . .",
	". . . . . . W . . . . . . . . . . . . .",
	". . . . . . W . . . . . . . . . . . . .",
	". . . W W W W . . . . . . . . . . . . .",
	". . . . . . . . . . . . . . . . E . . ."
};
// 地图
char(*MAP)[20] = new char[10][20];
// 地图最大尺寸
Point * MAX_PNT = new Point(10, 20);
//// 起点
Point *START_PNT = nullptr;
//// 终点
Point * END_PNT = nullptr;


//从当前点走到末尾需要有的代价，可以有上述不同的估算方式
//此类封装了不同的估值方法
class  evaluateCost
{
public:
	/**
	* 曼哈顿距离,小于等于实际值
	*/
	static double hManhattanDistance(Point pnt)
	{
		return abs(pnt.x - END_PNT->x) + abs(pnt.y - END_PNT->y);
	}

	/**
	* 欧式距离,小于等于实际值
	*/
	static double hEuclidianDistance(Point pnt)
	{
		return  sqrt(pow(pnt.x - END_PNT->x, 2) + pow(pnt.y - END_PNT->y, 2));
	}

	/**
	* 欧式距离平方,大于等于实际值
	*/
	static double hPowEuclidianDistance(Point pnt)
	{
		return pow(pnt.x - END_PNT->x, 2) + pow(pnt.y - END_PNT->y, 2);
	}

	/**
	* BFS的h值,恒为0
	*/
	static double hBFS(Point pnt)
	{
		return 0;
	}
};

void CreateMap();
void printMap();
void searchPath();
void main()
{
	 //根据字符串创建图	
	CreateMap();
	 //打印图
	 printMap();
	//A*算法寻找路径
	 searchPath();
	 //打印图
	 printMap();
	 system("pause");
}


//根据字符串创建图	
void CreateMap()
{
	for (int k = 0; k < 10; k++) {
		const  char *pstr = S_MAP[k].c_str();
		int index = 0;
		for (int i = 0; i < 39; i++)
		{
			if (pstr[i] == ' ')
			{
				continue;
			}
			MAP[k][index] = pstr[i];
			switch (pstr[i])
			{
			case START:
				START_PNT = new Point(k, index);
			case END:
				END_PNT = new Point(k, index);
			default:
				break;
			}
			index++;
		}
	}
}
//打印图
void printMap()
{

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			cout << MAP[i][j] << " ";
		}
		cout << endl;
	}
}
//A*算法寻找路径
void searchPath()
{

	//二叉堆对象
	minHeap heap;

	Data * head = new Data(*START_PNT, 0, 0, nullptr);//创建起始点
	heap.add(head); // 把起始点放入堆

					//移动的方向，前，后，左，右
	int indirect[4][2] = { { 1,0 },{ -1,0 },{ 0,1 },{ 0,-1 } };

	//存储最后一个节点
	Data  * lastData = nullptr;

	// 核心代码
	//如果没有到末尾 并且二叉堆的元素不为空，就一直继续下去
	//二叉堆的元素不为空，表明还有可以走的点。
	for (bool finish = false; !finish && !heap.isEmpty();)
	{
		//目前f值最小的元素，走到此点。
		Data * data = heap.getAndRemoveMin();
		//此点的坐标
		Point point = data->point;
		//将此点标示为已经走过
		if (MAP[point.x][point.y] == SPACE)
		{
			MAP[point.x][point.y] = VISITED;
		}

		//分别分析从该点出发的4个方向
		for (auto indir : indirect)
		{
			//新的坐标
			Point * newPnt = new Point(point.x + indir[0], point.y + indir[1]);

			//新的坐标x,y在Map中，有效
			if (newPnt->x >= 0 && newPnt->x < MAX_PNT->x && newPnt->y >= 0 && newPnt->y < MAX_PNT->y) {
				//新的坐标在map中的值
				char ch = MAP[newPnt->x][newPnt->y];

				//代表新的坐标就是结束点，结束查找
				if (ch == END)
				{
					cout << "找到" << endl;
					lastData = new Data(*newPnt, 0, 0, data);
					finish = true;
					break;
				}
				//代表新的坐标不为空，它可能已经走过了，也可能是墙。此点就不用再分析了。
				if (ch != SPACE)
				{
					continue;
				}

				//查找开启列表中是否存在该元素
				Data * finddata = heap.find(*newPnt);

				//存在
				if (finddata != nullptr)
				{
					/*
					*
					* data->g + 1：data其实是当前点，注意此1代表了从当前的data点到达newPnt即新点的代价，这里方便用1来表示，实际中要根据实际情况调整
					* 判断g值，如果此点的g值 比data->g + 1还大。说明了从当前点到达此此newPnt消耗的代价更小，所以我需要改变此newPnt的代价为最小的data->g + 1
					*并且将其父节点调整为data元素。
					*/
					if (finddata->g > data->g + 1)
					{
						finddata->g = data->g + 1;
						finddata->parent = data;
					}
				}
				//开启列表不存在，就直接添加进二叉堆中。
				else
				{
					//不同的估值函数求h，即到达结束点的代价。调用evaluateCost类中的静态成员函数
					double  db = (double)(evaluateCost::hManhattanDistance(*newPnt));
					Data * addData = new Data(*newPnt, data->g + 1, db, data);
					heap.add(addData);
				}
			}
		}
	}

	//从最后的end 一直查找追随路径，到达起点，就是走过最短的路
	//将此最短路径标示为ON_PATH
	for (Data * last = lastData; last != nullptr;)
	{
		Point pnt = last->point;
		if (MAP[pnt.x][pnt.y] == VISITED)
		{
			MAP[pnt.x][pnt.y] = ON_PATH;
		}
		last = last->parent;
	}
}
