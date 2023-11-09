#El tiempo de ordenamiento promedio en el Radix Sort seg´un el n´umero de bits k y el tama˜no
#del universo.

import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline

data = pd.read_csv("koptimos.csv",sep=";")

print(data.columns)
#Index(['ValorK', 'valor del n del universo', 'tiempo tardado'], dtype='object')

PromedioN = []
i = 0
for n in range(1,65):
    PromedioN.append(data['tiempo tardado'][i:i+25].mean())
    i += 25

x = range(1,65)
y = PromedioN

X_Y_Spline = make_interp_spline(x, y)
 
# Returns evenly spaced numbers
# over a specified interval.
X_ = np.linspace(1, 65, 500)
Y_ = X_Y_Spline(X_)

plt.plot(X_, Y_,)
plt.plot(range(1,65), PromedioN, 'ro')
plt.xlabel('Valor de n')
plt.ylabel('Tiempo de ejecución (s)')
plt.title('Tiempo de ejecución promedio de Radixsort segun el tamaño del universo')
plt.savefig('RadixN.png')
plt.show()

Pk = open("PromediosK.txt", "r")

PromedioK = []
for n in range(0,26):
    k = float(Pk.readline())
    PromedioK.append(k)

plt.plot(range(1,27), PromedioK, 'ro')
plt.xlabel('Valor de k')
plt.ylabel('Tiempo de ejecución (s)')
plt.title('Tiempo de ejecución promedio de Radixsort segun el numero de bits k')
plt.savefig('Koptimo.png')
plt.show()

#I have a csv file with 3 columns: k, n, time, I wish to get the best k for each n, where best is least amount of time

df = pd.read_csv('koptimos.csv', sep=';')
df = df.sort_values(by=['tiempo tardado'])
df = df.groupby('valor del n del universo').first().reset_index()
print(df)

# Create a table visualization of the DataFrame
fig, ax = plt.subplots(figsize=(6, 4))
ax.axis('tight')
ax.axis('off')
ax.table(cellText=df.values, colLabels=df.columns, cellLoc='center', loc='center')

# Save the table as a PNG image
plt.savefig('table.png', bbox_inches='tight', pad_inches=0.1, format='png')
