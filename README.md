# ProyectoComunicaciones

El proyecto consiste en la monitorización del CO2 de una habitación mediante un sensor conectado a una Raspberry Pi 3b+
Los datos se adquieren por Bluetooth y se envían a un programa C que los lee y convierte a archivos .csv

Una vez adquiridos los datos se construye otro archivo csv que contiene la información sobre la ganancia de CO2 en la habitación en unas condiciones determinadas. 
Este modelo ARX se pasa por un algoritmo de regresión lineal multivariable, implementado mediante el algoritmo de Descenso de Gradiente. 

La función lineal obtenida puede ser utilizada para tratar de predecir el comportamiento del habitáculo en condiciones que no han podido ser testeadas mediante el sistema de medición del que se dispone
