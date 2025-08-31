
public class Hebras extends Thread {
    private final String[][] matriz;
    private final int inicioX, inicioY, finX, finY;
    private int[] resultado;
    
    public Hebras(String[][] matriznueva, int inicioX, int inicioY, int finX, int finY){
        this.matriz=matriznueva;
        this.inicioX=inicioX;
        this.inicioY=inicioY;
        this.finX=finX;
        this.finY=finY;
        this.resultado=null;
    }

    @Override
    public void run(){
        int filas = finX - inicioX;
        int columnas = finY - inicioY;
        if(filas == 1 && columnas == 1) {
            resultado = buscarletra(matriz, inicioX, inicioY);
        }
        else{
            int midX = (inicioX + finX - 1) / 2;
            int midY = (inicioY + finY - 1) / 2;

            Hebras tarea1 = new Hebras(matriz, inicioX,inicioY,midX, midY);       // Cuadrante superior izquierdo
            Hebras tarea2 = new Hebras(matriz, midX+1, inicioY, finX, midY);       // Cuadrante superior derecho
            Hebras tarea3 = new Hebras(matriz, inicioX, midY+1, midX, finY);      // Cuadrante inferior izquierdo
            Hebras tarea4 = new Hebras(matriz, midX+1, midY+1, finX, finY);      // Cuadrante inferior derecho

            //Comenzar las hebras
            tarea1.start();
            tarea2.start();
            tarea3.start();
            tarea4.start();
            
            // Esperar que los threads terminen y obtener el resultado
            try {
                tarea1.join();
                tarea2.join();
                tarea3.join();
                tarea4.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            // Si se encontro la letra en algún cuadrante, retornar la posición
            if (tarea1.getResult() != null) {
                resultado = tarea1.getResult();
            } else if (tarea2.getResult() != null) {
                resultado = tarea2.getResult();
            } else if (tarea3.getResult() != null) {
                resultado = tarea3.getResult();
            } else if (tarea4.getResult() != null) {
                resultado = tarea4.getResult();
            }
        }
    }

    private int[] buscarletra(String[][] matriz, int inicioX, int inicioY) {
        for(int i = inicioX; i < inicioX + 2; i++){
            for(int j = inicioY; j < inicioY + 2; j++){
                if(!matriz[i][j].equals("0")) {
                    return new int[]{i, j};
                }
            }
        }
        return null;
    }

    public int[] getResult() {
        return resultado;
    }

}