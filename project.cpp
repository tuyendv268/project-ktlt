#include<bits/stdc++.h>
using namespace std;

struct UserInput{  
    int num_sensors ;
    int sampling;
    int interval;  
};

struct Time{
    int hour;
    int minute;
    int second;
};

struct Sensor{
    int id;
    Time time;
    int values;
};

bool is_number(char *);
void write_data_to_csv_file(vector<Sensor> );
void display(vector<Sensor> );
vector<Sensor> generate_dataset(UserInput );
string convert_time_to_string(Time );
Time convert_second_to_time(int );
int convert_time_to_second(Time );
Time get_current_time();
Time add_time(Time , Time );
int parse_argument(vector<string> , const string );


bool is_number(string str){
    int size = str.length();
    for(int i = 0; i < size; i++){
        if(isdigit(str[i]) == false){
            return false;
        }
    }
    return true;
}

int parse_argument(vector<string> argvs, const string keyword){
    int index = -1;
    for(int i = 0; i < argvs.size(); i++){
        if(argvs[i] == keyword){
            cout<<"find : "<<argvs[i]<<endl;
            index = i;
            if(index == argvs.size() - 1){
                return -1;
            }
            break;
        }
    }
    if(index >= argvs.size()){
        return 0;
    }
    if(is_number(argvs[index+1])){
        return stoi(argvs[index+1]);
    }else{
        return -1;
    }
}

Time add_time(Time time1, Time time2){
    Time result;
    int second = time1.second + time2.second;
    result.second = second % 60;
    int minute = (int)(second/60) + time1.minute + time2.minute;
    result.minute = minute % 60;
    int hour = (int)(minute/60) + time1.hour + time2.hour;
    result.hour = hour;

    return result;
}

Time get_current_time(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    Time time;

    time.hour = ltm->tm_hour;
    time.minute = ltm->tm_min;
    time.second = ltm->tm_sec;
    return time;
}
int convert_time_to_second(Time time){
    return time.hour*3600 + time.minute*60 + time.second;
}

Time convert_second_to_time(int second){
    Time time;
    time.hour = (int) second/3600;
    time.minute = (int)((second - time.hour*3600)/60);
    time.second = second - time.hour*3600 - time.minute*60;
    return time;
}

string convert_time_to_string(Time time){
    string hour = (time.hour >= 10)?to_string(time.hour): ("0"+to_string(time.hour));
    string minute = (time.minute >= 10)?to_string(time.minute):("0"+to_string(time.minute));
    string second = (time.second >= 10)?to_string(time.second):("0"+to_string(time.second));
    return hour + ":" + minute + ":" + second;
}

vector<Sensor> generate_dataset(UserInput user_input){
    // use current time as seed for random generator
    srand(time(0));
    vector<Sensor> sensors; 
    Sensor sensor_tmp;
    Time current_time, sampling_time;
    int number_of_sampling = user_input.interval/user_input.sampling;

    current_time = get_current_time();
    for(int i = 1; i <= number_of_sampling; i++){
        for(int j = 1; j <= user_input.num_sensors; j++){
            sensor_tmp.id = j;
            sensor_tmp.time = current_time;
            sensor_tmp.values = rand();
            sensors.push_back(sensor_tmp);
        }
        sampling_time = convert_second_to_time(user_input.num_sensors*user_input.sampling);
        current_time =  add_time(current_time, sampling_time);
    }
    return sensors;
}

void display(vector<Sensor> sensors){
    for(int i = 0; i < sensors.size();i++){
        cout<<sensors[i].id<<","<<convert_time_to_string(sensors[i].time)<<","<<sensors[i].values<<endl;
    }
}

void write_data_to_csv_file(vector<Sensor> sensors){
    int starting_time = convert_time_to_second(sensors[0].time);
    string file_name = "speed_data_"+to_string(starting_time)+".csv";
    FILE *file_pointer;
    file_pointer = fopen(file_name.c_str(), "w+");
    fprintf(file_pointer,"id, time, values\n");
    for(int i = 0; i < sensors.size();i++){
        int id = sensors[i].id;
        string time = convert_time_to_string(sensors[i].time);
        int values = sensors[i].values;
        fprintf(file_pointer,"%d, %s, %d\n", id, time.c_str(), values);
    }
    fclose(file_pointer);
}

int main(int argc, char** argv){
    UserInput user_input;
    vector<string> argvs(argv, argv+argc);

    int num_sensors = parse_argument(argvs, "-n");
    if(num_sensors == 0){
        user_input.num_sensors = 1;
    }else if(num_sensors == -1){
        cout<<"you haven't enter value for -n argument !"<<endl;
        return 0;
    }else{
        user_input.num_sensors = num_sensors;
    }
    cout<<user_input.num_sensors<<endl;

    int sampling = parse_argument(argvs, "-st");
    if(sampling == 0){
        user_input.sampling = 10;
    }else if(sampling == -1){
        cout<<"you haven't enter value for -st argument !"<<endl;
        return 0;
    }else{
        user_input.sampling = sampling;
    }
    cout<<user_input.sampling<<endl;
    
    int interval = parse_argument(argvs, "-si");
    if(interval == 0){
        user_input.interval = 3600;
    }else if(interval == -1){
        cout<<"you haven't enter value for -si argument !"<<endl;
        return 0;
    }else{
        user_input.interval = interval;
    }
    cout<<user_input.interval<<endl;
    vector<Sensor> sensors = generate_dataset(user_input);
    write_data_to_csv_file(sensors);
    // display(sensors);
    return 0;
}