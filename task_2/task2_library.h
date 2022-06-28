#pragma once
#include<iostream>
#include<bits/stdc++.h>
#include<vector>
using namespace std;

typedef struct Time{
    int hour;
    int minute;
    int second;
}Time;

typedef struct Sensor{
    int id;
    Time time;
    float values;
}Sensor;

Time parse_time(char*);
void print_sensors(vector<Sensor>);
string convert_time_to_string(Time);
vector<Sensor> read_csv(string);
vector<Sensor> read_csv_1(string);
vector<Sensor> get_invalid_sensors(string, int, int);
vector<Sensor> get_valid_sensors(string, int, int);
void write_sensors_to_csv_file(vector<Sensor> , string);

void print_sensors(vector<Sensor> sensors){
    for(int i = 0; i < sensors.size();i++){
        cout<<sensors[i].id<<","<<convert_time_to_string(sensors[i].time)<<","<<sensors[i].values<<endl;
    }
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

Time subtract_time(Time time1, Time time2){
    int t1 = convert_time_to_second(time1);
    int t2= convert_time_to_second(time2);
    Time res = (t1 > t2)?convert_second_to_time(t1-t2):convert_second_to_time(t2-t1);
    return res;
}


Time get_earlier_time(Time time1, Time time2){
    int t1 = convert_time_to_second(time1);
    int t2 = convert_time_to_second(time2);
    return (t1 > t2)?time2:time1;
}

string convert_time_to_string(Time time){
    string hour = (time.hour >= 10)?to_string(time.hour): ("0"+to_string(time.hour));
    string minute = (time.minute >= 10)?to_string(time.minute):("0"+to_string(time.minute));
    string second = (time.second >= 10)?to_string(time.second):("0"+to_string(time.second));
    return hour + ":" + minute + ":" + second;
}

void write_sensors_to_csv_file(vector<Sensor> sensors, string file_name){
    FILE *file_pointer;
    file_pointer = fopen(file_name.c_str(), "a");
    fprintf(file_pointer,"id, time, values\n");
    for(int i = 0; i < sensors.size();i++){
        int id = sensors[i].id;
        string time = convert_time_to_string(sensors[i].time);
        float values = sensors[i].values;
        fprintf(file_pointer,"%d, %s, %0.1f\n", id, time.c_str(), values);
    }
    fclose(file_pointer);
}

Time parse_time(char* time){
    Time output_time;
    int time_arr[3], count = 0;
    string temp="";
    for(int i = 0; i < strlen(time); i++){
        if(time[i] == ':'){
            time_arr[count] = atoi(temp.c_str());
            count += 1;
            temp = "";
        }else{
            temp = temp + time[i];
        }
    }
    time_arr[count] = atoi(temp.c_str());

    output_time.hour = time_arr[0];
    output_time.minute = time_arr[1];
    output_time.second = time_arr[2];
    // cout<<time_arr[0]<<"-"<<time_arr[1]<<"-"<<time_arr[2]<<endl;
    // cout<<output_time.hour<<"\n"<<output_time.minute<<"\n"<<output_time.second<<endl;
    return output_time;
}

vector<Sensor> get_invalid_sensors(string file_name, int low, int high){
    // cout<<file_name<<endl;
    vector<Sensor> outlier_sensors, sensors = read_csv(file_name);
    // print_sensors(sensors);
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].values < low || sensors[i].values > high){
            Sensor sensor_tmp;
            sensor_tmp.id = sensors[i].id;
            sensor_tmp.time = sensors[i].time;
            sensor_tmp.values = sensors[i].values;

            outlier_sensors.push_back(sensor_tmp);
        }
    }
    return outlier_sensors;
}

vector<Sensor> get_valid_sensors(string file_name, int low, int high){
    // cout<<file_name<<endl;
    vector<Sensor> outlier_sensors, sensors = read_csv(file_name);
    // print_sensors(sensors);
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].values >= low && sensors[i].values <= high){
            Sensor sensor_tmp;
            sensor_tmp.id = sensors[i].id;
            sensor_tmp.time = sensors[i].time;
            sensor_tmp.values = sensors[i].values;

            outlier_sensors.push_back(sensor_tmp);
        }
    }
    return outlier_sensors;
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
        // cout<<value<<endl;
        Sensor sensor_tmp;
        while (value) {
            if (column == 0) {
                sensor_tmp.id = atoi(value);
                // cout<<sensor_tmp.id<<endl;
            }
            if (column == 1) {
                // cout<<value<<endl;
                // char tmp[50];
                // strcpy(tmp, value);
                sensor_tmp.time = parse_time(value);
                // cout<<"log"<<endl;
                // cout<<sensor_tmp.time.hour<<":"<<sensor_tmp.time.minute<<":"<<sensor_tmp.time.second<<endl;
            }
            if (column == 2) {
                sensor_tmp.values = stod(value);
                // cout<<value<<endl;
            }
            value = strtok(NULL, ", ");
            column++;
        }
        sensors.push_back(sensor_tmp);
    }
    fclose(fp);
    return sensors; 
}