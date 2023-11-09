#Nuestro primer objetivo es rellenar lo que no se logro experimentar, es decir, el Quicksort para n > 26

archivo = open("ResultadosFinales.csv", "r") # Archivo base
archivoModificado = open("ResultadosFinalesModificados.csv", "w")  # Nuevo archivo con info nueva

linea = archivo.readline()          # 'linea' será la variable que lea el archivo de entrada
archivoModificado.write(linea)
linea = archivo.readline()        # Escribimos la primera linea en el archivo modificado

Diferencia = 0
Quicksort_anterior = 0
n_anterior = 1

while linea != "":                  # Mientras no lleguemos al final del archivo
    infoDeLinea = linea.split(";")    # Separamos la linea en una lista de strings, separados por ';'
    Quicksort = infoDeLinea[0]        # 'Quicksort' es el tiempo de ejecución de Quicksort

    if Quicksort == "None" and n_anterior != infoDeLinea[2]:
        newQuicksort = Diferencia*2 + Quicksort_anterior
        newQuicksort = str(newQuicksort)
        nuevaLinea = (f"{newQuicksort};{infoDeLinea[1]};{infoDeLinea[2] };{infoDeLinea[3]};{infoDeLinea[4]}")
        Diferencia = float(newQuicksort) - Quicksort_anterior
        Quicksort_anterior = float(newQuicksort)
        archivoModificado.write(nuevaLinea)
    
    elif Quicksort == "None" and n_anterior == infoDeLinea[2]:
        newQuicksort = Quicksort_anterior
        newQuicksort = str(newQuicksort)
        nuevaLinea = (f"{newQuicksort};{infoDeLinea[1]};{infoDeLinea[2] };{infoDeLinea[3]};{infoDeLinea[4]}")
        archivoModificado.write(nuevaLinea)

    elif Quicksort != "None" and n_anterior != infoDeLinea[2]:
        Diferencia = float(Quicksort) - Quicksort_anterior
        nuevaLinea = (f"{Quicksort};{infoDeLinea[1]};{infoDeLinea[2] };{infoDeLinea[3]};{infoDeLinea[4]}")
        archivoModificado.write(nuevaLinea)
        Quicksort_anterior = float(Quicksort)
    
    else:
        nuevaLinea = (f"{Quicksort};{infoDeLinea[1]};{infoDeLinea[2] };{infoDeLinea[3]};{infoDeLinea[4]}")
        archivoModificado.write(nuevaLinea)
        Quicksort_anterior = float(Quicksort)
        
    n_anterior = infoDeLinea[2]
    linea = archivo.readline()        # Seguimos leyendo

archivo.close()
archivoModificado.close()

#Ahora queremos graficar el tiempo de ejecución promedio de Quicksort y Radixsort para cada valor de n en 2^n
import pandas as pd 
import matplotlib.pyplot as plt

data = pd.read_csv("ResultadosFinalesModificados.csv", sep=';')

print(data.columns)
#Index(['Resultado Quicksort', 'Resultado RadixSort', 'Valor n en 2^n','n° prueba', 'valor k radixsort'],dtype='object')

PromedioQuicksort = []
PromedioRadixsort = []

i = 0
for n in range(0,64):
    PromedioQuicksort.append(data['Resultado Quicksort'][i:i+100].mean())
    PromedioRadixsort.append(data['Resultado RadixSort'][i:i+100].mean())
    i += 100

#Graficamos

plt.plot(range(1,65), PromedioQuicksort, label = "Quicksort")
plt.plot(range(1,65), PromedioRadixsort, label = "Radixsort")
plt.title('Tiempo de ejecución promedio de Quicksort vs Radixsort')
plt.xlabel('Valor de n en 2^n')
plt.ylabel('Tiempo de ejecución (s)')
plt.legend()
plt.savefig('comp.png')
plt.show()

plt.plot(range(1,65), PromedioQuicksort, label = "Quicksort")
plt.title('Tiempo de ejecución promedio de Quicksort')
plt.xlabel('Valor de n en 2^n')
plt.ylabel('Tiempo de ejecución (s)')
plt.savefig('quick.png')
plt.show()

plt.plot(range(1,65), PromedioRadixsort, label = "Radixsort")
plt.title('Tiempo de ejecución promedio de Radixsort')
plt.xlabel('Valor de n en 2^n')
plt.ylabel('Tiempo de ejecución (s)')
plt.savefig('radix.png')
plt.show()
