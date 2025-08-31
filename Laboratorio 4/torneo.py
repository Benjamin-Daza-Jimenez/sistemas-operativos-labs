import threading
import random
import time
from datetime import datetime
import os # Eliminar archivos

# Variables globales
fase_actual = "Validacion"
contador_global=0
termino = False

# Semáforos para los dos grupos de validación (32 cada uno)
validacion_grupo1 = threading.Semaphore(32)
validacion_grupo2 = threading.Semaphore(32)

# Candados
lock_contador = threading.Lock()
lock_fase = threading.Lock()
lock_validacion = threading.Lock()
lock_eliminacion = threading.Lock()

rondas_ganadores = [[],[],[],[],[],[],[],[]]
rondas_perdedores = [[],[],[],[],[],[],[],[],[],[]]
ganadores = []
perdedores = []
finalistas = []

class Jugador(threading.Thread):
    # Constructor
    def __init__(self, id):
        super().__init__()
        self.id = id
        self.estado = "Esperando"
        self.ganador = False
        self.ronda_ganadores = 0
        self.ronda_perdedores = 0

    def run(self):
        global fase_actual, rondas_ganadores, rondas_perdedores, finalistas
        # Fase de Validación
        self.validar()
        # Esperar a que comience la fase de eliminación
        while True:
            with lock_fase:
                if fase_actual == "Eliminacion":
                    break
            time.sleep(0.1)

        # Fase de Eliminación
        time.sleep(10) 
        while True:
            if self.estado == "En competencia":
                self.eliminacion()
            else: break
        
        # Fase de Repechaje
        time.sleep(10)
        while True:
            if self.estado == "En repechaje":
                self.repechaje()
            else: break

        # Final
        while True:
            if self.estado == "Finalista":
                self.final()
            else: break

    def validar(self):
        global fase_actual, rondas_ganadores, rondas_perdedores, contador_global

        #Asigna el grupo según orden de llegada
        with lock_contador:
            contador_global += 1
            if(contador_global%2==0):grupo=validacion_grupo1
            else:grupo=validacion_grupo2

        #grupo = validacion_grupo1 if random.choice([True, False]) else validacion_grupo2
        with grupo:
            time.sleep(15)
            with lock_validacion:
                entrada = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                validar = open("Resultado/Validación.txt",'a')
                validar.write(f"{self.id} "+entrada+"\n")
                validar.close()
                rondas_ganadores[0].append(self)
            self.estado = "En competencia"

    def eliminacion(self):
        global fase_actual, rondas_ganadores, rondas_perdedores, finalistas
        with lock_eliminacion:
            entrada = datetime.now().strftime("%H:%M:%S.%f")[:-3]
            ronda = self.ronda_ganadores
            if(ronda > 7): return
            if self.estado == "En repechaje":
                return # Sali si ya perdio
            if len(rondas_ganadores[ronda]) < 2:
                return # Espera a más jugadores
            oponente = rondas_ganadores[ronda].pop()
            if self == oponente:
                rondas_ganadores[ronda].insert(0, oponente) # Evitar duelo consigo mismo
                return
            # Simular duelo
            ganador = random.choice([self, oponente])
            perdedor = self if ganador == oponente else oponente
            # Escribir en el txt
            eliminar = open(f"Resultado/Ganadores_Ronda{ronda+1}.txt", "a")
            eliminar.write(f"{self.id} vs {oponente.id} {entrada}, Ganador: {ganador.id}\n")
            eliminar.close()
            # Eliminar al ganador y perdedor de las rondas
            if ganador in rondas_ganadores[ronda]:
                rondas_ganadores[ronda].remove(ganador)
            if perdedor in rondas_ganadores[ronda]:
                rondas_ganadores[ronda].remove(perdedor)
            # Actualziar a sus nuevas rondas
            ganador.ronda_ganadores += 1 #El ganador pasa a la siguiente ronda
            perdedor.ronda_perdedores = ronda #El perdedor va a la misma ronda pero a perderdores
            # Agregar a las rondas
            if(ronda != 7):
                rondas_ganadores[ronda + 1].append(ganador)
                ganador.estado = "En competencia"
            else: 
                finalistas.append(ganador)
                ganador.estado = "Finalista"
            rondas_perdedores[ronda].append(perdedor)
            perdedor.estado = "En repechaje"
        time.sleep(10)

    def repechaje(self):
        global fase_actual, rondas_perdedores, finalistas
        with lock_eliminacion:
            entrada = datetime.now().strftime("%H:%M:%S.%f")[:-3]
            ronda = self.ronda_perdedores
            if(ronda > 9): return
            if self.estado == "Eliminado":
                return # Sali si ya perdio
            if len(rondas_perdedores[ronda]) < 2:
                return # Espera a más jugadores
            oponente = rondas_perdedores[ronda].pop()
            if self == oponente:
                rondas_perdedores[ronda].insert(0, oponente) # Evitar duelo consigo mismo
                return
            # Simular duelo
            ganador = random.choice([self, oponente])
            perdedor = self if ganador == oponente else oponente
            # Escribir en el txt
            eliminar = open(f"Resultado/Perdedores_Ronda{ronda+1}.txt", "a")
            eliminar.write(f"{self.id} vs {oponente.id} {entrada}, Ganador: {ganador.id}\n")
            eliminar.close()
            # Eliminar al ganador y perdedor de las rondas
            if ganador in rondas_perdedores[ronda]:
                rondas_perdedores[ronda].remove(ganador)
            if perdedor in rondas_perdedores[ronda]:
                rondas_perdedores[ronda].remove(perdedor)
            # Actualziar a sus nuevas rondas
            ganador.ronda_perdedores += 1 #El ganador pasa a la siguiente ronda
            # Agregar a las rondas
            if(ronda != 9):
                rondas_perdedores[ronda + 1].append(ganador)
                ganador.estado = "En repechaje"
            else: 
                finalistas.append(ganador)
                ganador.estado = "Finalista"
            perdedor.estado = "Eliminado"
        time.sleep(10)

    def final(self):
        global finalistas, termino
        with lock_eliminacion:
            entrada = datetime.now().strftime("%H:%M:%S.%f")[:-3]
            if len(finalistas) < 2:
                return # Espera a más jugadores
            oponente = finalistas.pop()
            if self == oponente:
                    finalistas.insert(0, oponente) # Evitar duelo consigo mismo
                    return
            # Simular duelo
            ganador = random.choice([self, oponente])
            perdedor = self if ganador == oponente else oponente
            # Escribir en el txt
            final = open(f"Resultado/Final.txt", "a")
            final.write(f"{self.id} vs {oponente.id} {entrada}, Ganador: {ganador.id}\n")
            final.close()
            ganador.estado = "Campeon"
            perdedor.estado = "Eliminado"
            termino = True

### Coordinador del torneo
def torneo():
    global fase_actual, rondas_ganadores, rondas_perdedores, termino
    # Eliminar archivos
    if not os.path.exists("Resultado"):
        os.makedirs("Resultado")
    if os.path.exists("Resultado/Validación.txt"):
        os.remove("Resultado/Validación.txt")
    for i in range(8):
        if os.path.exists(f"Resultado/Ganadores_Ronda{i+1}.txt"):
            os.remove(f"Resultado/Ganadores_Ronda{i+1}.txt")
    for i in range(8):
        if os.path.exists(f"Resultado/Perdedores_Ronda{i+1}.txt"):
            os.remove(f"Resultado/Perdedores_Ronda{i+1}.txt")

    # Inicia hebras
    jugadores = [Jugador(f"Hebra-{i+1}") for i in range(256)]
    for j in jugadores:
        j.start()

    # Fase de Validación
    time.sleep(15)  # Tiempo para que se complete la validación
    with lock_fase:
        fase_actual = "Eliminacion"
        
    # Final
    while termino == False:
        time.sleep(10)

    # Finalizar todas las hebras
    for j in jugadores:
        j.join()

# Ejecuta el torneo
torneo()
