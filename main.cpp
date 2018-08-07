// Warsaw Rhubarbs
#include <bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

typedef long long ll;

int dist(const pair<int,int> & a, const pair<int,int> & b) {
	return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<string> split(const string &s, char delimiter){
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while(getline(ss,item,delimiter)){
		tokens.push_back(item);
	}
	return tokens;
}

struct Ride {
	pair<int,int> start, end;
	int when_start, when_end;
	int id;
	void read(int _id) {
		id = _id;
//		cin >> start.first >> start.second >> end.first >> end.second;
//		cin >> when_start >> when_end;
	}
	int length() const {
		return dist(start, end);
	}
};

int bonus_start;

vector<vector<int>> lines;
vector<vector<int>> output;
int super;
struct Car {
	pair<int,int> where;
	int when;
	int id;
	int when_can_get(const pair<int,int> & a) const {
		return when + dist(where, a);
	}
	int real_simulate(const Ride & ride, bool indeed) {
		ll score = 0;
		when += dist(where, ride.start);
		if(when <= ride.when_start) {
			if(indeed) ++super;
			when = ride.when_start;
			score += bonus_start;
		}
		when += ride.length();
		if(when <= ride.when_end) {
			score += ride.length();
		}
		else {
			assert(!indeed);
			score = 0;
		}
		where = ride.end;
		return score;
	}
	int fake_simulate(const Ride & ride) const {
		Car x = *this;
		return x.real_simulate(ride, false);
	}
	int would_wait(const Ride & ride) const {
		int get_there = when_can_get(ride.start);
		return max(0, ride.when_start - get_there);
	}
	int would_need(const Ride & ride) const {
		return max(when_can_get(ride.start), ride.when_start) - when;
	}
	void add_ride(const Ride & ride) const {
		output[id].push_back(ride.id);
	}
};


int main(int argc, char** argv) {
	cout << "Thanks to warsaw rhubarbs for uploading their source code to help other people optimize their code" << endl ;
	cout << "--------------------------------------------------------------------------------------------------" << endl << endl ;
	string filename = "c_no_hurry.in";
	int startInfo[6];
	if(argc > 1) filename = argv[1];
//	else filename = argv[argc - 1];
	cout << filename << endl;
	string line;
	ifstream inputFile;
	inputFile.open(filename);
	if(inputFile.is_open()){
		int counter = 0;
		while(getline(inputFile,line)){
//			cout<< line << endl;
			vector<string> myData = split(line,' ');
//			int inputLine[6];
//			cout<<myData[0]<<endl;
			if(counter == 0){
				startInfo[0] = stoi(myData[0]);
				startInfo[1] = stoi(myData[1]);
				startInfo[2] = stoi(myData[2]);
				startInfo[3] = stoi(myData[3]);
				startInfo[4] = stoi(myData[4]);
				startInfo[5] = stoi(myData[5],nullptr,10);
			}else{
//				cout<<stoi(myData[0])<<endl;
				vector<int> rideInfo;
				rideInfo.push_back(stoi(myData[0]));
//				cout<<myData[1]<<endl;
				rideInfo.push_back(stoi(myData[1]));
//				cout<<myData[2]<<endl;
				rideInfo.push_back(stoi(myData[2]));
//				cout<<myData[3]<<endl;
				rideInfo.push_back(stoi(myData[3]));
//				cout<<myData[4]<<endl;
				rideInfo.push_back(stoi(myData[4]));
//				cout<<myData[5]<<endl;
				rideInfo.push_back(stoi(myData[5]));
				
				lines.push_back(rideInfo);
			}
			
			counter++;
		}
		inputFile.close();
	}else{
		cerr << "Error opening";
		return 1;
	}
	cout<<line<<endl;
	int h, w, cnt_cars, cnt_rides;
	int T;
//	cin >> h >> w >> cnt_cars >> cnt_rides >> bonus_start >> T;
	h = startInfo[0];
	w = startInfo[1];
	cnt_cars = startInfo[2];
	cnt_rides = startInfo[3];
	bonus_start = startInfo[4];
	T = startInfo[5];
	output.resize(cnt_cars);
	
	vector<Ride> rides(cnt_rides);
	cout << "Initializing rides"<<endl;
	for(int i = 0; i < cnt_rides; ++i){
//		cout<<i<<endl;
//		cin>>T;
		vector<int> ride_details = lines[i];
		rides[i].start.first = ride_details[0];
		rides[i].start.second = ride_details[1];
		rides[i].end.first = ride_details[2];
		rides[i].end.second = ride_details[3];
		rides[i].when_start = ride_details[4];
		rides[i].when_end = ride_details[5];
		rides[i].read(i);
	}
	cout << "Done!" << endl;
	
	vector<Car> cars(cnt_cars);
	for(int i = 0; i < cnt_cars; ++i)
		cars[i].id = i;
		
	vector<bool> done(cnt_rides);
	
	int total_score = 0;
	int AC = 0;
	
	vector<int> far(cnt_rides, INT_MAX);
	for(int i = 0; i < cnt_rides; ++i) if(!done[i])
		for(int j = 0; j < cnt_rides; ++j) if(i != j) if(!done[j])
			far[i] = min(far[i], dist(rides[i].end, rides[j].start));
	
	while(true) {
		bool anything = false;
		
		for(Car & car : cars) {
			pair<int, int> best;
			best.first = INT_MAX;
			for(int i = 0; i < cnt_rides; ++i) 
				if(!done[i]) {
					int would_score = car.fake_simulate(rides[i]);
					if(would_score == 0) continue; // can't do it in time
					int wasted = car.would_need(rides[i]);
					int when_would_finish = car.when + wasted + rides[i].length();
                              // change the 0.75 till you get the best optimized version
					if(when_would_finish <= T * 0.48) wasted += far[i] / 15;
					best = min(best, {wasted, i});
				}
				if(best.first == INT_MAX) continue;
			int i = best.second;
			done[i] = true;
			int tmp = car.real_simulate(rides[i], true);
			total_score += tmp;
			anything = true;
			car.add_ride(rides[i]);
			++AC;
		}
		if(!anything) break;
	}
	cout << "Writing out configuration" << endl ;
	cerr << super << "/" << AC << "/" << cnt_rides << "\n";
	cerr << "optimization index = " << total_score / 1e6 << "\n";
	ofstream outfile;
	if(argc > 2) filename = argv[2];
	else filename = "c.out";
	outfile.open(filename);
	if(outfile.is_open()){
		for(int i = 0; i < cnt_cars; ++i) {
//			printf("%d", (int) output[i].size());
//			puts("");
			outfile << to_string(output[i].size());
			string ride_indexes = "";
			for(int x : output[i]) ride_indexes += " "+to_string(x);
			outfile << ride_indexes;
			outfile << '\n';
		}
	}
	outfile.close();
	cout << "Enjoy the points" << endl;
}
