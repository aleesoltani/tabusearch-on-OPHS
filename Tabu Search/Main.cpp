#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <Windows.h>
#include <iomanip>
#include <chrono>
using namespace std;


class Place
{
public : 
	bool isHotel;
	bool isStartingHotel;
	bool isEndingHotel;
	bool isCity;
	
	bool isSelected;
	int id;
	float X;
	float Y;
	int score;
	void elementInit(int inId, float inX, float inY, int inScore, bool isHot, bool isStart, bool isEnd)
	{
		id = inId;
		X = inX;
		Y = inY;
		score = inScore;
		isHotel = isHot;
		isStartingHotel = isStart;
		isEndingHotel = isEnd;
		isCity = !isHot;
		
		isSelected = false;
	}
};
float getDist(Place a, Place b);
class Trip
{
public:
	int placeCount = 0;
	float distance = 0.0;
	float length = 0.0;
	int score=0;
	vector<Place> route;
	vector<Place>::const_iterator itr;
	Place current;
	Place startHotel;
	Place endHotel;
	void distanceUpdate()
	{
		Place curr = this->startHotel;
		this->distance = 0.0;
		for (this->itr = this->route.begin();this->itr < this->route.end();++this->itr)
		{
			this->distance += getDist(curr, *this->itr);
			curr = *this->itr;
		}
		this->distance += getDist(curr, this->endHotel);
	}
	void scoreUpdate()
	{
		Place curr = this->startHotel;
		this->score = 0;
		for (this->itr = this->route.begin();this->itr < this->route.end();++this->itr)
		{
			this->score += this->itr->score;
		}
	}
	
};
class Tabu
{
public:
	Place place;
	bool isValid;
	int stay = 0;
};
class TabuList
{
public:
	int tabucount = 5;
	int countAdd=0;
	int countDel=0;
	Tabu addList[20];
	Tabu delList[20];
	void addTabu(Place PLACE,string WHAT)
	{
		if (WHAT == "add")
		{
			if (countAdd != tabucount)
			{
				addList[countAdd].place = PLACE;
				addList[countAdd].isValid = true;
				addList[countAdd].stay = tabucount;
				countAdd++;
			}
			else
			{
				int i = 0;
				bool added = false;
				for (i = 0;i < tabucount;i++)
				{
					if (addList[i].isValid = false)
					{
						addList[i].place = PLACE;
						addList[i].isValid = true;
						addList[i].stay = tabucount;
						added = true;
						break;
					}
				}
				if (added == false)
				{
					remove(addList);
					for (i = 0;i < tabucount;i++)
					{
						if (addList[i].isValid = false)
						{
							addList[i].place = PLACE;
							addList[i].isValid = true;
							addList[i].stay = tabucount;
							added = true;
							break;
						}
					}
				}
			}
		}
		else if (WHAT == "del")
		{
			if (countDel != tabucount)
			{
				delList[countDel].place = PLACE;
				delList[countDel].isValid = true;
				delList[countDel].stay = tabucount;
				countDel++;
			}
			else
			{
				int i = 0;
				bool added = false;
				for (i = 0;i < tabucount;i++)
				{
					if (delList[i].isValid = false)
					{
						delList[i].place = PLACE;
						delList[i].isValid = true;
						delList[i].stay = tabucount;
						added = true;
						break;
					}
				}
				if (added == false)
				{
					remove(delList);
					for (i = 0;i < tabucount;i++)
					{
						if (delList[i].isValid = false)
						{
							delList[i].place = PLACE;
							delList[i].isValid = true;
							delList[i].stay = tabucount;
							added = true;
							break;
						}
					}
				}
			}
		}
	}
	void next()
	{
		int i = 0;
		for (i = 0;i < tabucount;i++)
		{
			if (addList[i].isValid)
			{
				addList[i].stay--;
			}
			if (delList[i].isValid)
			{
				delList[i].stay--;
			}
			if (addList[i].stay == 0)
			{
				addList[i].isValid = false;
			}
			if (delList[i].stay == 0)
			{
				delList[i].isValid = false;
			}
		}
		
	}
	void remove(Tabu *a)
	{
		int i = 0;
		Tabu lowest;
		lowest.stay = tabucount;
		for (i = 0;i < tabucount;i++)
		{
			if (a[i].stay <= lowest.stay)
			{
				lowest = a[i];
			}
		}
		for (i = 0;i < tabucount;i++)
		{
			if (a[i].place.id == lowest.place.id)
			{
				a[i].isValid = false;
				break;
			}
		}
	}
	void clear()
	{
		int i;
		for (i = 0;i < tabucount;i++)
		{
			addList[i].isValid = false;
			delList[i].isValid = false;
		}
	}
	
};
class Solution
{
public:
	Trip _trips[20];
	int _score=0;
};
TabuList tabuList;
Solution best;
ofstream osFile;
bool changeHotel(int TRIPNUM);
bool isTabu(Place P);
void clear();
void tabuSearch();
void localSearch();
void order();
void constructRandomSolution();
void initFromFile(string inputFileName);
void getDir(wchar_t * dir);
void selectPlace(float remainingDist,int TRIPNUMBER);
void createInitial(float TRLen, int TRNum);
float getVecLen(vector<Place> VECTOR);
void setSelected(int id, bool what);
//----**********************************************----//^^ funcs
int fileCount = 0;
string inputList[40];
int totalScore = 0;
float totalTime = 0.0;
float timeForRun;
float bestTime;
int inputIterations;
int initialCount;
double timeElapsed;
bool ISANEWPLACESELECTED=false;
Place currentPlace;
Place places[130];
Place ordered[130];
Trip trips[20];
int totalPlacesInFile = 0;
int verticesPlusTwo, hotelsMinusTwo, numTrips;
int totalTourLen;
float tripLen[20];
float currTripLen;
int countPV = 0;

//----**********************************************----//^^ global variables
void main()
{
	osFile.open("out.txt");
	srand(time(NULL));
	int i = 0;
	int j;
	wchar_t spath[2048];
	//wsprintf(spath, L"D:\\Reference\\uni\\2nd\ Semester\\Advanced\ Algorithms\\Assignments\\AA-benchmark\\E\\*.*");
	//getDir(spath);
	//string address = "D:\\Reference\\uni\\2nd\ Semester\\Advanced\ Algorithms\\Assignments\\AA-benchmark\\E\\";
	//for (i = 0; i < fileCount; i++)
	//{
		//inputList[i] = address + inputList[i];
	//}
	//initFromFile(inputList[1]);
	
	string address = "D:\\Reference\\uni\\2nd\ Semester\\Advanced\ Algorithms\\Assignments\\AA-benchmark\\H\\100-240-12-4-1306.ophs";
	initFromFile(address);
	for (i = 0;i < numTrips;i++)
	{
		trips[i].length = tripLen[i];
	}
	order();
	cout << endl << "Please Enter Desired Number Of Initial Solutions :";
	cin >> initialCount;
	cout << endl << "Please Enter Desired Number Of Iterations On Each Solution :";
	cin >> inputIterations;
	tabuSearch();
	_getch();
}
void tabuSearch()
{
	

	int sumPlacesVisitedInTour=0;
	int i = 0;
	
	currentPlace = places[0];
	bool flagnew = false;
	for (int j = 0;j < initialCount;j++)
	{
		//*****
		//while for cunstructing initial hotels
		totalScore = 0;
		cout << "*********************"<<endl;
		flagnew = false;
		clear();
		while (trips[numTrips - 1].endHotel.id != places[1].id || !flagnew)
		{
			i = 0;
			while (i < numTrips)
			{
				currTripLen = tripLen[i];
				createInitial(currTripLen, i);
				flagnew = true;
				i++;
			}
			
		}//while for constructing initial hotels
		auto start = std::chrono::high_resolution_clock::now();
		
		currentPlace = trips[0].startHotel;
		constructRandomSolution();
		localSearch();
		auto elapsed = std::chrono::high_resolution_clock::now() - start;
		double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
		timeElapsed = milliseconds;
		totalTime += timeElapsed;
		cout << "Run time for this solution(miliSec): " << timeElapsed  << endl;
		if (j == 0) bestTime = timeElapsed;
		if (timeElapsed < bestTime)
		{
			bestTime = timeElapsed;
		}
		cout << "*********************" << endl;
	}
	int k = 0;
	cout << "Best Found :" << endl;
	osFile << "Best Found :" << endl;
	cout << endl;
	for (k = 0;k < numTrips;k++)
	{
		cout << endl << "trip[" << k << "] : ";
		osFile << endl << "trip[" << k << "] : ";
		for (best._trips[k].itr = best._trips[k].route.begin();best._trips[k].itr < best._trips[k].route.end();++best._trips[k].itr)
		{
			if (best._trips[k].itr != best._trips[k].route.end() - 1)
			{
				cout << best._trips[k].itr->id << ", ";
				osFile << best._trips[k].itr->id << ", ";
			}
			else
			{
				cout << best._trips[k].itr->id;
				osFile << best._trips[k].itr->id;
			}
		}
		cout << "\tdist: " << best._trips[k].distance << "\tLen: " << best._trips[k].length;
		osFile << "\tdist: " << best._trips[k].distance << "\tLen: " << best._trips[k].length;
	}
	cout << endl << best._score << endl;
	cout << endl << "Average run time for each solution(miliSec): " << totalTime * 1000 / initialCount << endl;
	cout << endl << "Best run time between solutions(miliSec): " << bestTime * 1000 << endl;
	osFile << endl <<"Score: "+ best._score << endl;
	osFile << endl << "Average run time for each solution(miliSec): " << totalTime * 1000 / initialCount << endl;
	osFile << endl << "Best run time between solutions(miliSec): " << bestTime * 1000 << endl;
}
void constructRandomSolution()
{
	int i;
	float distanceTraveled = 0.0;
	float newDist;
	float TRLen;
	int TRNum;
	
	countPV = 0;
	ISANEWPLACESELECTED = true;
	
	int scoreForTrip = 0;
	for (i = 0 ; i < numTrips ; i++)
	{
		
		TRLen = tripLen[i];
		//distanceTraveled = 0.0;
		scoreForTrip = 0;
		currentPlace = trips[i].startHotel;
		trips[i].route.push_back(trips[i].startHotel);
		ISANEWPLACESELECTED = true;
		while (trips[i].distance < TRLen && ISANEWPLACESELECTED)
		{
			ISANEWPLACESELECTED = false;
			selectPlace(TRLen - trips[i].distance,i);
			countPV++;
		}
		trips[i].route.push_back(trips[i].endHotel);
		// so now
		// a random answer has been created
		trips[i].distanceUpdate();
		trips[i].scoreUpdate();
		
		/*cout << endl << "trip #" << i << " : ";
		for (trips[i].itr = trips[i].route.begin();trips[i].itr < trips[i].route.end();++trips[i].itr)
		{
			if (trips[i].itr != trips[i].route.end() - 1)
			{
				cout << trips[i].itr->id << ", ";
			}
			else
			{
				cout << trips[i].itr->id;
			}
			totalScore += trips[i].itr->score;
			scoreForTrip += trips[i].itr->score;
		}
		trips[i].score = scoreForTrip;
		cout << "\tdist: " << trips[i].distance << "\ttripMax: " << tripLen[i];*/
		//cout << endl << scoreForTrip;
	}
	/*cout << endl << totalScore<< endl;*/
	
}
void selectPlace(float remainingDist,int TRIPNUM)
{
	//float returningFloat=0.0;
	int j = rand() % totalPlacesInFile;
	float myDistance = getDist(currentPlace, places[j]) + getDist(places[j], trips[TRIPNUM].endHotel);
	//cout << "a";
	int count = 0;
	while ( (myDistance > remainingDist || places[j].isHotel || places[j].isSelected) && count<50)
	{
		//we're stuck here###################
		//cout << ".";
		count++;
		j = rand() % ((totalPlacesInFile)-(hotelsMinusTwo+2) )+ hotelsMinusTwo+2;
		myDistance = getDist(currentPlace, places[j]) + getDist(places[j], trips[TRIPNUM].endHotel);
	}
	if (count != 50)
	{
		if (getDist(currentPlace,places[j]) + getDist(places[j],trips[TRIPNUM].endHotel) <= remainingDist && places[j].isCity && !places[j].isSelected)
		{
			trips[TRIPNUM].distance += getDist(currentPlace, places[j]);
			//returningFloat = getDist(currentPlace,places[j]);
			trips[TRIPNUM].route.push_back(places[j]);
			currentPlace = places[j];
			setSelected(j, true);
			trips[TRIPNUM].placeCount++;
			ISANEWPLACESELECTED = true;
		}
	}
	else
	{
		trips[TRIPNUM].distance += getDist(currentPlace, trips[TRIPNUM].endHotel);
		//returningFloat = getDist(currentPlace, trips[TRIPNUM].endHotel);
		//changeHotel();
	}
	//code
	
	//return returningFloat;
}
void localSearch()
{
	tabuList.clear();
	double sec=0;
	int i=0,j=0,k=0;
	int n = 0,m=0;
	int score = 0;
	float calculate=0.0;
	vector<Place> copyVector;
	vector<Place>::const_iterator iter;
	Place current;
	timeForRun = 0;
	auto s= std::chrono::system_clock::now();
	
	while (sec<60000)
	{
		score = 0;
		sec = 0;
		for (k = 0;k < numTrips;k++)
		{
			while (sec<1200)
			{
				tabuList.next();
				copyVector.clear();
				for (i = 0;i < trips[k].route.size();i++)
				{
					copyVector.push_back(trips[k].route.at(i));
				}
				//started dynamic route cities visited
				for (i = 0;i < trips[k].route.size();i++)
				{
					for (j = hotelsMinusTwo + 2;j < totalPlacesInFile;j++)
					{
						if (!ordered[j].isSelected && ordered[j].isCity && !isTabu(ordered[j]))
						{
							if (i != trips[k].route.size() - 1)
							{
								calculate = getVecLen(copyVector) + getDist(trips[k].route.at(i), ordered[j]) + getDist(ordered[j], trips[k].route.at(i + 1));
								if (calculate <= trips[k].length)
								{
									copyVector.insert(copyVector.begin() + (i + 1), ordered[j]);
									setSelected(ordered[j].id, true);
									tabuList.addTabu(ordered[j], "add");
									break;
								}
							}
							else
							{
							
							}
						}
					}
				}
				trips[k].route.clear();
				for (i = 0;i < copyVector.size();i++)
				{
					trips[k].route.push_back(copyVector.at(i));
				}
				trips[k].distanceUpdate();
				trips[k].scoreUpdate();

				for (i = 0;i < trips[k].route.size();i++)
				{
					for (j = hotelsMinusTwo + 2;j < totalPlacesInFile;j++)
					{
						if (!ordered[j].isSelected && ordered[j].score > trips[k].route.at(i).score && !isTabu(ordered[j]))
						{
							if (i != 0 && i != trips[k].route.size() - 1)
							{
								calculate = trips[k].distance - (getDist(trips[k].route.at(i - 1), trips[k].route.at(i)) + getDist(trips[k].route.at(i), trips[k].route.at(i + 1)));
								calculate = calculate + (getDist(trips[k].route.at(i - 1), ordered[j]) + getDist(ordered[j], trips[k].route.at(i + 1)));
								if (calculate <= trips[k].length)
								{
									setSelected(trips[k].route.at(i).id, false);
									tabuList.addTabu(trips[k].route.at(i), "del");
									tabuList.addTabu(ordered[j], "add");
									ordered[j].isSelected = true;
									setSelected(ordered[j].id, true);
									trips[k].route.at(i) = ordered[j];
									trips[k].distanceUpdate();
									trips[k].scoreUpdate();

								}
							}
							else if (i != 0)
							{
								if (k < numTrips - 2)
								{
									bool b=false;
									for (int o = 0;o < 20&&b==false ;o++)
										b=changeHotel(k);
								}
							}
						}
					}
				}
				auto e = std::chrono::system_clock::now() - s;
				sec = std::chrono::duration_cast<std::chrono::milliseconds>(e).count();
			}
		}
		auto e = std::chrono::system_clock::now() - s;
		sec = std::chrono::duration_cast<std::chrono::milliseconds>(e).count();
	}
	score = 0;
	for (k = 0;k < numTrips;k++)
	{
		cout << endl << "trip[" << k << "] : ";
		for (trips[k].itr = trips[k].route.begin();trips[k].itr < trips[k].route.end();++trips[k].itr)
		{
			if (trips[k].itr != trips[k].route.end() - 1)
			{
				cout << trips[k].itr->id << ", ";
			}
			else
			{
				cout << trips[k].itr->id;
			}
		}
		cout << "\tdist: " << trips[k].distance << "\tLen: " << trips[k].length;
		score += trips[k].score;
		
	}
	cout << endl << score <<endl;
	//
	if (score > best._score)
	{
		for (i = 0;i < numTrips;i++)
		{
			best._trips[i] = trips[i];
			best._score = score;
		}
	}
	//
	
}
void initFromFile(string inputFileName)
{
	int i = 0;
	int j = 0;
	float sX;
	float sY;
	int sScore = 0;
	float deltaX;
	float deltaY;
	ifstream isFile;
	isFile.open(inputFileName);
	isFile >> verticesPlusTwo;
	isFile >> hotelsMinusTwo;
	isFile >> numTrips;
	isFile >> totalTourLen;
	totalPlacesInFile = verticesPlusTwo + hotelsMinusTwo;
	for (i = 0; i < numTrips; i++)
	{
		isFile >> tripLen[i];
	}
	cout << endl << "fromFILE:" + inputFileName << endl;
	for (i = 0; i < totalPlacesInFile; i++)
	{
		isFile >> sX;
		isFile >> sY;
		isFile >> sScore;
		if (sScore == 0)
		{
			if (i == 0) { places[i].elementInit(i, sX, sY, sScore, true, true, false); }
			else if (i == 1) { places[i].elementInit(i, sX, sY, sScore, true, false, true); }
			else
			{
				places[i].elementInit(i, sX, sY, sScore, true, false, false);
			}
		}
		else
		{
			places[i].elementInit(i, sX, sY, sScore, false, false, false);
		}
	}
	cout << endl;
	isFile.close();
	/*for (i = 0; i < totalPlacesInFile; i++)
	{
		dDistance[i][i] = 0;
		for (j = i; j < totalPlacesInFile; j++)
		{
			if (i != j)
			{
				deltaX = places[i].X - places[j].X;
				deltaY = places[i].Y - places[j].Y;
				dDistance[i][j] = sqrtf(powf(deltaX, 2) + powf(deltaY, 2));
				dDistance[j][i] = dDistance[i][j];
			}
		}
	}*/

}
float getDist(Place a, Place b)
{
	float dist = 0;
	float deltaX;
	float deltaY;
	float power;
	deltaX = a.X - b.X;
	deltaY = a.Y - b.Y;
	power = powf(deltaX, 2) + powf(deltaY, 2);
	dist = sqrtf(power);
	return dist;
}
void getDir(wchar_t *dir)
{
	WIN32_FIND_DATA findData;
	HANDLE hFile;
	wchar_t spath[2048];
	wcscpy_s(spath, dir);
	hFile = FindFirstFile(spath, &findData);
	FindNextFile(hFile, &findData);
	wstring wSToConv(findData.cFileName);
	string fileName(wSToConv.begin(), wSToConv.end());
	int count = 0;
	while (FindNextFile(hFile, &findData))
	{

		wSToConv.assign(findData.cFileName);
		fileName.assign(wSToConv.begin(), wSToConv.end());
		inputList[count] = fileName;
		count++;
	}
	fileCount = count;

}
void createInitial(float TRLen,int TRNum)
{
	int j = rand() % (hotelsMinusTwo + 2);
	int localCount = 0;
	if (TRNum == 0)
	{
		trips[TRNum].startHotel = places[0];
	}
	if (TRNum == (numTrips - 1))
	{
		if (getDist(trips[TRNum].startHotel,places[1]) <= TRLen)
		{
			trips[TRNum].endHotel = places[1];
		}
		else
		{
			trips[TRNum].endHotel = trips[TRNum].startHotel;
			//cout << endl << "no answer, problem in hotel selection for past trips" << endl;
		}
	}
	if (numTrips - TRNum > 4)
	{
		localCount = 0;
		j = rand() % (hotelsMinusTwo + 2);
		while ((getDist(places[j],trips[TRNum].startHotel) > TRLen || places[j].isCity ) && localCount < (hotelsMinusTwo + 2))//|| getDist(places[j], places[1]) > getDist(trips[TRNum].startHotel, places[1])
			//ADD THAT last part to get closer to 1 from the start :-)
		{
			j = rand() % (hotelsMinusTwo + 2);
			localCount++;
		}
		if (places[j].isHotel && (getDist(places[j],trips[TRNum].startHotel) <= TRLen))
		{
			trips[TRNum].endHotel = places[j];
			trips[TRNum+1].startHotel = trips[TRNum].endHotel;
		}
		else
		{
			trips[TRNum].endHotel = trips[TRNum].startHotel;
			trips[TRNum+1].startHotel = trips[TRNum].endHotel;

		}
	}
	if (numTrips - TRNum > 1 && numTrips - TRNum <= 4)
	{
		localCount=0;
		j= rand() % (hotelsMinusTwo + 2);
		while ((getDist(places[j],trips[TRNum].startHotel) > TRLen || places[j].isCity || getDist(places[j],places[1]) >= getDist(trips[TRNum].startHotel,places[1]) || getDist(places[j],places[1]) > tripLen[TRNum + 1] )&& localCount<100)
		{
			j = rand() % (hotelsMinusTwo + 2);
			localCount++;
		}
		if (getDist(places[j],places[1]) <= getDist(trips[TRNum].startHotel,places[1]) || getDist(places[j],places[1])<=tripLen[TRNum+1])
		{
			
			trips[TRNum].endHotel = places[j];
			trips[TRNum+1].startHotel = trips[TRNum].endHotel;
		}
		else if(localCount ==100 && getDist(trips[TRNum].startHotel,places[1]) <= TRLen)
		{
		
			trips[TRNum].endHotel = places[1];
			trips[TRNum+1].startHotel = trips[TRNum].endHotel;
		}
	}
	
}
void clear()
{
	int i = 0;
	for (i = 0;i < numTrips;i++)
	{
		trips[i].route.clear();
		trips[i].placeCount = 0;
		trips[i].distance = 0.0;
	}
	for (i = 0;i < totalPlacesInFile;i++)
	{
		places[i].isSelected = false;
		ordered[i].isSelected = false;
	}
}
float getVecLen(vector<Place> VECTOR)
{
	float ret=0.0;
	int i = 0;
	vector<Place>::const_iterator it;
	for (i = 0;i < VECTOR.size()-1;i++)
	{
		ret += getDist(VECTOR.at(i), VECTOR.at(i + 1));
	}
	return ret;
}
void order()
{
	int i = 0,j=0;
	Place temp;
	for (i = 0;i < totalPlacesInFile;i++)
	{
		ordered[i] = places[i];
	}
	for (i = hotelsMinusTwo + 2; i < totalPlacesInFile; i++)
	{
		for (j = i+1; j < totalPlacesInFile;j++)
		{
			if (ordered[j].score > ordered[i].score && ordered[i].isCity && ordered[j].isCity)
			{
				temp = ordered[i];
				ordered[i] = ordered[j];
				ordered[j] = temp;
			}
		}
	}
}
void setSelected(int id,bool what)
{
	int j = 0;
	for (j = 0;j < totalPlacesInFile;j++)
	{
		if (ordered[j].id == id)
		{
			ordered[j].isSelected = what;
		}
		if (places[j].id == id)
		{
			places[j].isSelected = what;
		}
	}
}
bool isTabu(Place P)
{
	int i = 0;
	for (i = 0;i < tabuList.tabucount;i++)
	{
		if ((tabuList.addList[i].place.id == P.id && tabuList.addList[i].isValid) || (tabuList.delList[i].place.id == P.id && tabuList.delList[i].isValid))
		{
			return true;
		}
	}
	return false;
}
bool changeHotel(int TRIPNUM)
{
	int i = 0;
	int j = rand() % (hotelsMinusTwo + 2);
	while (places[j].isCity)
	{
		j = rand() % (hotelsMinusTwo + 2);
	}
	float dist = trips[TRIPNUM].distance;
	float dist2 = trips[TRIPNUM + 1].distance;
	dist = dist - getDist(trips[TRIPNUM].route.at(trips[TRIPNUM].route.size() - 2), trips[TRIPNUM].route.at(trips[TRIPNUM].route.size() - 1));
	dist = dist + getDist(trips[TRIPNUM].route.at(trips[TRIPNUM].route.size() - 2), places[j]);
	dist2 = dist2 - getDist(trips[TRIPNUM + 1].startHotel, trips[TRIPNUM+1].route.at(1));
	dist2 = dist2 + getDist(places[j], trips[TRIPNUM+1].route.at(1));
	if (dist <= trips[TRIPNUM].length && dist2 <= trips[TRIPNUM + 1].length)
	{
		trips[TRIPNUM].endHotel = places[j];
		trips[TRIPNUM].route.at(trips[TRIPNUM].route.size() - 1) = places[j];
		trips[TRIPNUM + 1].startHotel = places[j];
		trips[TRIPNUM + 1].route.at(0) = places[j];
		trips[TRIPNUM].distanceUpdate();
		trips[TRIPNUM + 1].distanceUpdate();
		return true;
	}
	return false;
}