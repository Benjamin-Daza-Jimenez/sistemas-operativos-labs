Integrante 1: Benjamín Daza Jiménez | Rol: 202173574-7 | Paralelo 200
Integrante2: Sebastián Von Kunowsky Lepe | Rol: 202173560-7 | Paralelo 200

Especificación:
- El MAKE tiene como nombre "makefile".
- El archivo c++ tiene como nombre "Lab2.cpp".
- Se usó Ubuntu para pruebas.
- Crea el mazo de cartas dentro del programa.
- EL programa tiene problemas de sincronización/memoria compartida. Debido a que no se actualizan los vectores al terminar cada proceso (pila descarte).

Instrucciones: 
- Se debe tener instalado make y el compilador g++.
- Desde la terminal usar el comando "make run", este debe ejecutar el makefile.
- Se mostrará por pantalla que se crea el mazo y las manos del jugador y los bots.
- En cada turno, se mostrará la última carta de la pila de descarte, a su vez, las posibles cartas que puedes tirar.
- Solo puedes sacar una carta del mazo por turno. 
- Una vez terminado el juego, la memoria es liberada y los procesos terminan.

Consideraciones:
- Si el mazo queda con 0 cartas, el juego terminará. El ganador se decide por quien tiene la menor cantidad de cartas.
- Si un bot no tiene cartas para tirar, sacará una del mazo automaticamente y probará si puede usarla, en caso que no pueda, pasará turno.
- Los +2 y +4 se corroboran al inicio de cada turno, ya que se manejan mediante memoria compartida. Estos no se pueden acumular.


