#include "task2_library.h"

typedef struct summary{
    Sensor max;
    Sensor min;
    Sensor mean;
}summary;

Sensor find_min(vector<Sensor> sensors, int id, float left_value,float right_value);
Sensor find_max(vector<Sensor> sensors, int id, float left_value,float right_value);
Sensor find_average(vector<Sensor> sensors, int id, float left_value,float right_value);


vector<summary> get_data_summary(string file_name, float left_value, float right_value){
    vector<Sensor> sensors = get_valid_sensors(file_name, left_value, right_value);
    Sensor max_sensor, min_sensor, average_sensor;
    vector<summary> output_sensor;

    vector<Sensor> tmp = read_csv(file_name);
    Time  sampling_time = subtract_time(tmp[0].time, tmp[1].time);
    // cout<<convert_time_to_string(tmp[0].time)<<endl;
    // cout<<convert_time_to_string(tmp[1].time)<<endl;
    // cout<<convert_time_to_string(sampling_time)<<endl;
    int sampling_time_tmp = convert_time_to_second(sampling_time);
    Time simulation_interval = convert_second_to_time(sampling_time_tmp*tmp.size());
    
    set<int> unique_id;
    set<int>::iterator itr;

    for(int i = 0; i < sensors.size(); i++){
        unique_id.insert(sensors[i].id);
    }
    summary temp;

    for (itr = unique_id.begin(); itr != unique_id.end(); itr++) {
        max_sensor = find_max(sensors, *itr, left_value, right_value);
        min_sensor = find_min(sensors, *itr, left_value, right_value);
        average_sensor = find_average(sensors, *itr, left_value, right_value);
        average_sensor.time = simulation_interval;

        temp.max = max_sensor;
        temp.min = min_sensor;
        temp.mean = average_sensor;
        output_sensor.push_back(temp);
        // output_sensor.push_back(max_sensor);
        // output_sensor.push_back(min_sensor);
        // output_sensor.push_back(average_sensor);
    }
    return output_sensor;
}


void write_data_summary_to_csv(vector<summary> sensors, string file_name){
    FILE *file_pointer;
    int id;
    string time;
    float values;
    file_pointer = fopen(file_name.c_str(), "w");

    fprintf(file_pointer,"id, parameters, time, values\n");
    for(int i = 0; i < sensors.size();i++){
        id = sensors[i].max.id;
        time = convert_time_to_string(sensors[i].max.time);
        values = sensors[i].max.values;
        fprintf(file_pointer,"%d, %s, %s, %0.1f\n", id, "max",time.c_str(), values);

        id = sensors[i].min.id;
        time = convert_time_to_string(sensors[i].min.time);
        values = sensors[i].min.values;
        fprintf(file_pointer,"%d, %s, %s, %0.1f\n", id, "min",time.c_str(), values);
        
        id = sensors[i].mean.id;
        time = convert_time_to_string(sensors[i].mean.time);
        values = sensors[i].mean.values;
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

Sensor find_min(vector<Sensor> sensors, int id, float left_value, float right_value){
    Sensor min_sensor;
    min_sensor.values = 99999.0;
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].id == id && sensors[i].values >= left_value && sensors[i].values <=right_value){
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
Sensor find_max(vector<Sensor> sensors, int id, float left_value, float right_value){
    Sensor max_sensor;
    max_sensor.values = -1.0;
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].id == id && sensors[i].values >= left_value && sensors[i].values <=right_value){
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

Sensor find_average(vector<Sensor> sensors, int id, float left_value, float right_value){
    Sensor average_sensor;
    average_sensor.values = 0;
    int total = 0;
    for(int i = 0; i < sensors.size(); i++){
        if(sensors[i].id == id && sensors[i].values >= left_value && sensors[i].values <=right_value){
            average_sensor.values += sensors[id].values;
            total ++;
        }
    }
    average_sensor.values = average_sensor.values/total;
    average_sensor.id = id;
    return average_sensor;
}

int main(int argc, char ** argv){
    string file_name;
    cout<<argv[1]<<endl;
    if(argc < 2){
        file_name = "speed_data.csv";
    }else{
        file_name = string(argv[1]);
    }
    // cout<<file_name<<endl;
    float left_value=900.0, right_value=1600.0;
    cout<<"hello"<<endl;
    vector<summary> sensors = get_data_summary(file_name,left_value, right_value);
    cout<<"hello"<<endl;
    write_data_summary_to_csv(sensors, "data_summary.csv");
    cout<<"hello"<<endl;
    return 0;
}