import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("resultados.csv", sep=',')
#Index(['tamaño del vector ', 'tiempo tardado Sweep ',
#       'tiempo tardado random Universal ', 'tiempo tardado random FMR'],
#      dtype='object')

ValoresN = []
PromedioSweep = []
PromedioUni = []
PromedioFMR = []

i = 1
for n in range(0,10):
    ValoresN.append(data['tamaño del vector '][i:i+4].mean())
    PromedioSweep.append(data['tiempo tardado Sweep '][i:i+4].mean())
    PromedioUni.append(data['tiempo tardado random Universal '][i:i+4].mean())
    PromedioFMR.append(data['tiempo tardado random FMR'][i:i+4].mean())
    i += 5

#Time plot of the average time of execution of each algorithm
#Sweep
plt.figure()

plt.plot(ValoresN, PromedioSweep)
plt.plot(ValoresN, PromedioSweep, 'ro')
plt.xlabel('Valor de n')
plt.ylabel('Tiempo de ejecución (s)')
plt.title('Tiempo de ejecución promedio de Sweep segun el tamaño del vector')

plt.savefig('resultados/gSweepN.png')
plt.show()

plt.figure()

#Universal
plt.figure()

plt.plot(ValoresN, PromedioUni)
plt.plot(ValoresN, PromedioUni, 'ro')
plt.xlabel('Valor de n')
plt.ylabel('Tiempo de ejecución (s)')
plt.title('Tiempo de ejecución promedio de Random Universal segun el tamaño del vector')

plt.savefig('resultados/gUniversalN.png')
plt.show()

#FMR
plt.figure()

plt.plot(ValoresN, PromedioFMR)
plt.plot(ValoresN, PromedioFMR, 'ro')
plt.xlabel('Valor de n')
plt.ylabel('Tiempo de ejecución (s)')
plt.title('Tiempo de ejecución promedio de Random FMR segun el tamaño del vector')

plt.savefig('resultados/gFMRN.png')
plt.show()

# Load the CSV file into a DataFrame
file_path = 'resultados.csv'
df = pd.read_csv(file_path)

# List of attributes for which you want to generate histograms
attributes_to_plot = ['tiempo tardado Sweep ', 'tiempo tardado random Universal ', 'tiempo tardado random FMR']

# Loop through each attribute
for attribute_name in attributes_to_plot:
    # Create a histogram
    plt.figure()
    plt.hist(df[attribute_name], bins=10, color='blue', edgecolor='black')

    # Customize the plot
    plt.title(f'Histograma del {attribute_name}')
    plt.xlabel(attribute_name)
    plt.ylabel('Frecuencia')

    # Save the plot as an image (optional)
    plt.savefig(f'resultados/{attribute_name}_histogram.png')

    # Show the plot
    plt.show()