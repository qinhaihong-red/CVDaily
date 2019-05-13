#include "cv_helper.h"
#include <fstream>
#include <algorithm>
class CPlayer
{
public:
	CPlayer(std::string name="", int age=-1, int rank=-1);
	~CPlayer() {}
	//any function that do not modify the data member,should be declared and implemented as const function
	//which then can be passed as const reference
	void write(cv::FileStorage &fs) const;
	void read(const cv::FileNode &fnode);
	friend std::ostream& operator<<(std::ostream &os, CPlayer &player);
private:
	std::string _name;
	int _age;
	int _rank;
};

CPlayer::CPlayer(std::string name/* ="" */, int age/* =-1 */, int rank/* =-1 */):
_name(name),_age(age),_rank(rank)
{}

void CPlayer::write(cv::FileStorage &fs) const
{
	fs << "{" << "name" << _name << "age" << _age << "rank" << _rank << "}";
}

void CPlayer::read(const cv::FileNode &fnode)
{
	_age  = (int)fnode["age"];
	_rank = (int)fnode["rank"];
	_name = (std::string)fnode["name"];
}

static void read(const cv::FileNode &fnode, CPlayer &player, const CPlayer &_player = CPlayer())
{
	if (fnode.empty())
	{
		player = _player;
	}
	else
	{
		player.read(fnode);
	}
}

static void write(cv::FileStorage &fs, const std::string &,const CPlayer &player)
{
	player.write(fs);
}


static std::ostream& operator<<(std::ostream &os, CPlayer &player)
{
	os << "player name:" << player._name << ", age:" << player._age << ", rank:" << player._rank << std::endl;
	return os;
}


void cv_write_yaml(const std::string &file_name)
{
	try
	{
		cv::FileStorage fs(file_name, cv::FileStorage::WRITE);
		_CHECK_FAILURE(fs.isOpened());


		fs << "number" << 11;
		fs << "seq"<<"[";
		fs << "jordan" << 23;
		fs << "]";
		fs << "map";
		fs << "{" << "Lebron" << 23;
		fs << "curry" << 30 << "}";
		CPlayer player("durant", 30, 2);
		fs << "durant" << player;
		cv::Mat M = cv::Mat_<uchar>::eye(3, 3);
		fs <<"Matrix"<<M;

		fs.release();
	}
	catch (const cv::Exception& e)
	{
		EXCEPTION_INFO(e.what());
		_EXIT_FAILURE;
	}

}

void cv_read_yaml(const std::string &file_name)
{
	cv::FileStorage fs(file_name, cv::FileStorage::READ);
	CV_Assert(fs.isOpened());

	int number;
	fs["number"] >> number;
	std::cout << "number is:" << number << std::endl;

	cv::FileNode fnod = fs["seq"];
	if (fnod.type() != cv::FileNode::SEQ)
	{
		std::cout << "can not find 'seq'\n";
	}
	else
	{
		std::cout << "\nseq is:\n";
		for (auto iter = fnod.begin();iter!=fnod.end();++iter)
		{
			std::cout << (std::string)*iter << std::endl;
		}
	}

	fnod = fs["map"];
	if (fnod.type()!= cv::FileNode::MAP)
	{
		std::cout << "can not find 'map'\n";
	}
	else
	{
		std::cout << "\nmap is:\n";
		std::cout << "Lebron:" << (int)fs["Lebron"] << std::endl;
		std::cout << "curry:" << (int)fs["curry"] << std::endl;
	}

	CPlayer player;
	fs["durant"] >> player;
	cv::Mat m;
	fs["Matrix"] >> m;
	std::cout << "\ndurant is:" << player << std::endl;
	std::cout << "\nMatrix is:\n";
	PRINTMAT_F(m, NPF);

}



static int main_input_output()
{
	std::string file_name = cv_helper::get_localTimeStr();
	file_name += ".yaml";
	{
		std::ifstream ifs(file_name);
		if (!ifs)
		{
			cv_write_yaml(file_name);
		}
	}

	CV_TRY_CATCH(cv_read_yaml(file_name));

	return 0;
}