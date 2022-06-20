#include<iostream>
#include<bits/stdc++.h>
using namespace std;

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

Time parse_time(char* time){
    char* value = strtok(time, ", ");
    int index = 0;
    Time output_time;
    while (value) {
        if (index == 0) 
            output_time.hour = atoi(value);
        if (index == 1) 
            output_time.minute = atoi(value);
        if (index == 2)
            output_time.second = atoi(value);
        value = strtok(NULL, ":");
        index++;
    }
    return output_time;
}

string convert_time_to_string(Time time){
    string hour = (time.hour >= 10)?to_string(time.hour): ("0"+to_string(time.hour));
    string minute = (time.minute >= 10)?to_string(time.minute):("0"+to_string(time.minute));
    string second = (time.second >= 10)?to_string(time.second):("0"+to_string(time.second));
    return hour + ":" + minute + ":" + second;
}


void display(vector<Sensor> sensors){
    for(int i = 0; i < sensors.size();i++){
        cout<<sensors[i].id<<","<<convert_time_to_string(sensors[i].time)<<","<<sensors[i].values<<endl;
    }
}

vector<Sensor> read_csv(string file_name){
    FILE* fp = fopen(file_name.c_str(), "r");
    char buffer[1024];
    int row = 0, column = 0;
    vector<Sensor> sensors;
 
    while (fgets(buffer, 1024, fp)) {
        column = 0; row++;
        if (row == 1) continue;
        char* value = strtok(buffer, ", ");
        Sensor sensor_tmp;
        while (value) {
            if (column == 0) {
                sensor_tmp.id = atoi(value);
            }
            if (column == 1) {
                sensor_tmp.time = parse_time(value);
            }
            if (column == 2) {
                sensor_tmp.values = atoi(value);
            }
            value = strtok(NULL, ", ");
            column++;
        }
        sensors.push_back(sensor_tmp);
    }
    fclose(fp);
    return sensors; 
}

void write_csv_file(vector<Sensor> sensors, string file_name){
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

int main(int argc, char ** argv){
    string file_name;
    if(argc < 2){
        file_name = "speed_data.csv";
    }else{
        file_name = string(argv[1]);
    }
    cout<<file_name<<endl;
    vector<Sensor> sensors = read_csv(file_name);
    vector<Sensor> outlier_sensors;
    // display(sensors);
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].values >= 900 && sensors[i].values <= 1600){
            outlier_sensors.push_back(sensors[i]);
        }
    }
    write_csv_file(sensors, "outlier_data.csv");
    return 0;
}