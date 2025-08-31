//ForkJoinPool();
import java.util.concurrent.RecursiveTask;
   

class Forks extends RecursiveTask<int[]> {
    private final String[][] matriz;
    private final int inicioX, inicioY, finX, finY;

    public Forks(String[][] matriznueva, int inicioX, int inicioY, int finX, int finY){
        this.matriz=matriznueva;
        this.inicioX=inicioX;
        this.inicioY=inicioY;
        this.finX=finX;
        this.finY=finY;
    }

    @Override
    protected int[] compute(){
        //Buscar en matriz 2x2
        if(finX - inicioX == 1 && finY - inicioY == 1){
            for(int i = inicioX; i <= finX; i++){
                for(int j = inicioY; j <= finY; j++){
                    if(!matriz[i][j].equals("0")){
                        return new int[]{i, j};
                    }
                }
            }
            return null;
        }
        
        int midX = (inicioX + finX - 1) / 2;
        int midY = (inicioY + finY - 1) / 2;
        
        Forks tarea1 = new Forks(matriz, inicioX,inicioY,midX, midY);       // Cuadrante superior izquierdo
        Forks tarea2 = new Forks(matriz, midX+1, inicioY, finX, midY);       // Cuadrante superior derecho
        Forks tarea3 = new Forks(matriz, inicioX, midY+1, midX, finY);      // Cuadrante inferior izquierdo
        Forks tarea4 = new Forks(matriz, midX+1, midY+1, finX, finY);      // Cuadrante inferior derecho
        invokeAll(tarea1, tarea2, tarea3, tarea4);

        // Unir los resultados
        int[] resultado;
        resultado = tarea1.join();
        if(resultado != null) return resultado;
        resultado = tarea2.join();
        if(resultado != null) return resultado;
        resultado = tarea3.join();
        if(resultado != null) return resultado;
        resultado = tarea4.join();
        return resultado;
    }
}