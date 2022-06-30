from numpy import average
import pandas as pd

raw_data = {'schedule': [], 'dispatch': []}
for i in range(1, 8):
    dispatch = []
    with open('measures/' + str(i) + "tasks_dispatch.txt", 'r') as f:
        for line in f:
            dispatch.append(float(line))
    raw_data['dispatch'].append(dispatch)
    schedule = []
    with open('measures/' + str(i) + "tasks_schedule.txt", 'r') as f:
        for line in f:
            schedule.append(float(line))
    raw_data['schedule'].append(schedule)

calculated_data = {'schedule': [], 'dispatch': []}
for measure in raw_data['dispatch']:
    calculated = []
    for i in range(0, len(measure), 2):
        calculated.append(measure[i+1] - measure[i])
    calculated_data['dispatch'].append(calculated)
for measure in raw_data['schedule']:
    calculated = []
    for i in range(0, len(measure), 2):
        calculated.append(measure[i+1] - measure[i])
    calculated_data['schedule'].append(calculated)

average_data = {
    'schedule': [average(x) for x in calculated_data['schedule']], 
    'dispatch': [average(x) for x in calculated_data['dispatch']]
}

data = [(f"{i+1} tasks", average_data['dispatch'][i], average_data['schedule'][i]) for i in range(7)]

df = pd.DataFrame(data, columns=['# Tasks', 'Dispatch (average) (micros)', 'Schedule (average) (micros)'])

df.to_excel('measures.xlsx', index=False)

print(df)