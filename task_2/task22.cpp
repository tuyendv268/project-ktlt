#include "task2_library.h"

Sensor find_min(vector<Sensor> sensors, int id);
Sensor find_max(vector<Sensor> sensors, int id);
Sensor find_average(vector<Sensor> sensors, int id);

vector<Sensor> get_data_summary(string file_name){
    vector<Sensor> sensors = get_valid_sensors(file_name, 900, 1600);
    Sensor max_sensor, min_sensor, average_sensor;
    vector<Sensor> output_sensor;
    cout<<"hello"<<endl;

    cout<<sensors[0].id<<endl;
    Time  sampling_time = subtract_time(sensors[0].time, sensors[1].time);
    int sampling_time_tmp = convert_time_to_second(sampling_time);
    Time simulation_interval = convert_second_to_time(sampling_time_tmp*sensors.size());
    

    set<int> unique_id;
    set<int>::iterator itr;

    for(int i = 0; i < sensors.size(); i++){
        unique_id.insert(sensors[i].id);
    }
    

    for (itr = unique_id.begin(); itr != unique_id.end(); itr++) {
        max_sensor = find_max(sensors, *itr);
        min_sensor = find_min(sensors, *itr);
        average_sensor = find_average(sensors, *itr);
        average_sensor.time = simulation_interval;

        output_sensor.push_back(max_sensor);
        output_sensor.push_back(min_sensor);
        output_sensor.push_back(average_sensor);
    }
    return output_sensor;
}


void write_data_summary_to_csv(vector<Sensor> sensors, string file_name){
    FILE *file_pointer;
    int id;
    string time;
    float values;
    file_pointer = fopen(file_name.c_str(), "a");

    fprintf(file_pointer,"id, parameters, time, values\n");
    for(int i = 0; i < sensors.size();i++){
        id = sensors[i].id;
        time = convert_time_to_string(sensors[i].time);
        values = sensors[i].values;
        fprintf(file_pointer,"%d, %s, %s, %0.1f\n", id, "max",time.c_str(), values);

        id = sensors[i+1].id;
        time = convert_time_to_string(sensors[i+1].time);
        values = sensors[i+1].values;
        fprintf(file_pointer,"%d, %s, %s, %0.1f\n", id, "min",time.c_str(), values);
        
        id = sensors[i+2].id;
        time = convert_time_to_string(sensors[i+2].time);
        values = sensors[i+2].values;
        fprintf(file_pointer,"%d, %s, %s, %0.1f\n", id, "average",time.c_str(), values);
    }
    fclose(file_pointer);
}

int count_number_of_sensor(vector<Sensor> sensors){
    int min_id = INT_MAX, max_id = INT_MIN;
    for(int i = 0; i < sensors.size(); i++){
        if(min_id > sensors[i].id){
            min_id = sensors[i].id;
        }
        if(max_id < sensors[i].id){
            max_id = sensors[i].id;
        }
    }

    return max_id - min_id + 1;
}

Sensor find_min(vector<Sensor> sensors, int id){
    Sensor min_sensor;
    min_sensor.values = 99999.0;
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].id == id){
            if(min_sensor.values >= sensors[i].values){
                min_sensor = sensors[i];
                if(min_sensor.values == sensors[i].values){
                    int t1 = convert_time_to_second(min_sensor.time);
                    int t2 = convert_time_to_second(sensors[i].time);
                    min_sensor = (t1 > t2)?sensors[i]:min_sensor;
                }
            }
        }
    }
    return min_sensor;
}
Sensor find_max(vector<Sensor> sensors, int id){
    Sensor max_sensor;
    max_sensor.values = -1.0;
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].id == id){
            if(max_sensor.values <= sensors[i].values){
                max_sensor = sensors[i];
                if(max_sensor.values == sensors[i].values){
                    int t1 = convert_time_to_second(max_sensor.time);
                    int t2 = convert_time_to_second(sensors[i].time);
                    max_sensor = (t1 > t2)?sensors[i]:max_sensor;
                }
            }
        }
    }
    return max_sensor;
}

Sensor find_average(vector<Sensor> sensors, int id){
    Sensor average_sensor;
    average_sensor.values = 0;
    int total = 0;
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].id == id){
            average_sensor.values == sensors[id].values;
            total ++;
        }
    }
    average_sensor.values = average_sensor.values/total;
    average_sensor.id = id;
    return average_sensor;
}

int main(int argc, char ** argv){
    string file_name;
    if(argc < 2){
        file_name = "speed_data.csv";
    }else{
        file_name = string(argv[1]);
    }
    // cout<<file_name<<endl;
    vector<Sensor> sensors = get_data_summary(file_name);
    write_data_summary_to_csv(sensors, "data_summary.csv");
    return 0;
}