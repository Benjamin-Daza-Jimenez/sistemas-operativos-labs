import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.ForkJoinPool;

public class Lab3{
    //Trabajar con Forks
    public static int[] buscarFork(String[][] matriz){
        try(ForkJoinPool pool = new ForkJoinPool()){
            int[] resultado;

            Forks tarea = new Forks(matriz, 0, 0, matriz.length - 2,matriz.length - 2);
            resultado = pool.invoke(tarea);
            pool.shutdown();

            return resultado;
        }
    }
    //Trabajar con Hebras
    public static int[] buscarHebra(String[][] matriz){
        Hebras tarea = new Hebras(matriz, 0, 0, matriz.length - 2,matriz.length - 2);
        tarea.start();
        try{
            tarea.join(); // Esperar a que la hebra termine
        }catch(InterruptedException e){
            e.printStackTrace();
        }
        int[] resultado = tarea.getResult();
        return resultado;
    }

    public static void main(String[] args){

        List<List<String>> lista = new ArrayList<>(); //Se van a guardar las lineas de los archivos
        try (Scanner scanner = new Scanner(System.in)) {
            System.out.print("Introduce el nombre de la carpeta: ");
            String carpeta = scanner.nextLine();
            for(int i=1;i<10000000;i++){//Leer archivos .txt dentro de la carpeta
                String path="./archivos_prueba/"+carpeta+"/code"+i+".txt";
                
                try (FileReader fr = new FileReader(path)) {
                    BufferedReader br = new BufferedReader(fr);
                    
                    // Lectura del fichero
                    String linea;
                    List<String> aux = new ArrayList<>();
                    while((linea=br.readLine())!=null)
                        aux.add(linea);
                    lista.add(aux);
                }
                catch(Exception e){
                    break;
                }
            }
            
            System.out.println();System.out.println("----- Caso de prueba en Fork -----");System.out.println();
            
            String palabraFork = "";
            double totalduracion = 0.0;
            for(int i=0; i<lista.size();i++){
                List<String>archivo=lista.get(i);
                //Transforma de List<String> a String[][]
                String[][] matriz = new String[archivo.size()][archivo.size()];
                for(int j=1; j<archivo.size(); j++){
                    String linea=archivo.get(j);
                    String[] aux=linea.split(" ");
                    System.arraycopy(aux, 0, matriz[j-1], 0, aux.length);
                }
                
                long startTime = System.nanoTime();
                int[] posicion = buscarFork(matriz);
                String letra = matriz[posicion[0]][posicion[1]];
                long endTime = System.nanoTime();
                
                double duracion = (endTime - startTime) / 1_000_000.0;
                totalduracion += duracion;
                
                System.out.println("La letra encontrada para el code"+(i+1)+" es: "+letra);
                System.out.printf("El tiempo de ejecucion para el code%d es: %.4f[ms]%n",(i+1),duracion);System.out.println();
                
                palabraFork = palabraFork + letra;
            }
            System.out.println("Palabra resultante: "+ palabraFork);
            System.out.printf("Tiempo total de ejecucion es: %.4f[ms]%n",totalduracion);
            System.out.println();System.out.println();
            
            
            System.out.println("----- Caso de prueba en Hebras -----");
            
            totalduracion = 0.0;
            palabraFork = "";
            for(int i=0; i<lista.size();i++){
                List<String>archivo=lista.get(i);
                //Transforma de List<String> a String[][]
                String[][] matriz = new String[archivo.size()][archivo.size()];
                for(int j=1; j<archivo.size(); j++){
                    String linea=archivo.get(j);
                    String[] aux=linea.split(" ");
                    System.arraycopy(aux, 0, matriz[j-1], 0, aux.length);
                }
                
                long startTime = System.nanoTime();
                int[] posicion = buscarHebra(matriz);
                String letra = matriz[posicion[0]][posicion[1]];
                long endTime = System.nanoTime();
                
                double duracion = (endTime - startTime) / 1_000_000.0;
                totalduracion += duracion;
                
                System.out.println("La letra encontrada para el code"+(i+1)+" es: "+letra);
                System.out.printf("El tiempo de ejecucion para el code%d es: %.4f[ms]%n",(i+1),duracion);System.out.println();
                
                palabraFork = palabraFork + letra;
                
            }
            System.out.println("Palabra resultante: "+ palabraFork);
            System.out.printf("Tiempo total de ejecucion es: %.4f[ms]%n",totalduracion);
            System.out.println();System.out.println();
        }
    }
}

