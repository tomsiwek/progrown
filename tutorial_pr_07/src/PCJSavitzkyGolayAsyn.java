import java.io.IOException;
import java.util.Random;
import org.pcj.StartPoint;
import org.pcj.PCJ;
import org.pcj.NodesDescription;
import org.pcj.RegisterStorage;
import org.pcj.Storage;
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */
@RegisterStorage(PCJSavitzkyGolayAsyn.Shared.class)

public class PCJSavitzkyGolayAsyn implements StartPoint
{
    @Storage(PCJSavitzkyGolayAsyn.class)
    enum Shared {left, right};
    double left[] = new double [2];
    double right[] = new double [2];
    
    public static void main (String args[]) throws IOException
    {
        PCJ.start(PCJSavitzkyGolayAsyn.class, new NodesDescription("nodes.txt"));
    }
    
    @Override
    public void main() throws Throwable
    {
        int nAll = 1024;
        int n = nAll/PCJ.threadCount();
        double x[] = new double[n];
        double xprim[] = new double[n];
        Random r = new Random();

        // Inicjalizacja danych
        for (int i = 0; i < n; i++)
        {
            x[i] = r.nextDouble();
        }
        // prześlij lewą zakładkę do procesora z lewej strony
        if(PCJ.myId() != 0)
        {
            PCJ.asyncPut(x[0], PCJ.myId() - 1, Shared.right, 0);
            PCJ.asyncPut(x[1], PCJ.myId() - 1, Shared.right, 1);
        }
        // prześlij prawą zakładkę do procesora z prawej strony
        if(PCJ.myId() != PCJ.threadCount() - 1)
        {
            PCJ.asyncPut(x[n - 2], PCJ.myId() + 1, Shared.left, 0);
            PCJ.asyncPut(x[n - 1], PCJ.myId() + 1, Shared.left, 1);
        }
        
        // wygładzanie
        // obliczamy mianownik - czynnik normujący
        double a = 1.0/(-3 + 12 + 17 + 12 - 3);

        // obliczenia dla 2 pierwszych elementów z lewej strony (na 1. procesorze)
        if(PCJ.myId() == 0)
        {
            xprim[0] = (17*x[0] + 12*x[1] - 3*x[2]) / (17 + 12 - 3);
            xprim[1] = (12*x[0] + 17*x[1] + 12*x[2] - 3*x[3]) / (12 + 17 + 12 - 3);
        }
        // pobranie dwóch próbek na lewo z poprzedniego procesora
        else 
        {
            int p = PCJ.myId() - 1;
            PCJ.waitFor(Shared.left);
            xprim[0] = -3*((double)PCJ.get(p, Shared.left, 0)) + 12*((double)PCJ.get(p, Shared.left, 1)) + 17*x[0] + 12*x[1] - 3*x[2];
            xprim[0] = xprim[0] * a;
            xprim[1] = -3*((double)PCJ.get(p, Shared.left, 1)) + 12*x[0] + 17*x[1] + 12*x[2] - 3*x[3];
            xprim[1] = xprim[1] * a;
        }
        // obliczenia dla wszystkich elementów
        for (int i = 2; i < (n - 2); i++)
        {
            xprim[i] = -3*x[i - 2] + 12*x[i - 1] + 17*x[i] + 12*x[i + 1] - 3*x[i + 2];
            xprim[i] = xprim[i] * a;
        }
        
        // obliczenia na 2 ostatnich elementów (z prawej strony) (na ostatnim procesorze)
        if(PCJ.myId() == PCJ.threadCount() - 1)
        {
            xprim[n - 2] = (12*x[n - 1] + 17*x[n - 2] + 12*x[n - 3] - 3*x[n - 4]) / (12 + 17 + 12 - 3);
            xprim[n - 1] = (17*x[n - 1] + 12*x[n - 2] - 3*x[n - 3]) / (17 + 12 - 3);
        }
        // pobranie dwóch próbek na prawo z następnego procesora
        else
        {
            int p = PCJ.myId() + 1;
            PCJ.waitFor(Shared.right);
            xprim[n - 2] = -3*x[n - 4] + 12*x[n - 3] + 17*x[n - 2] + 12*x[n - 1] - 3*((double)PCJ.get(p, Shared.right, 0));
            xprim[n - 2] = xprim[n - 2] * a;
            xprim[n - 1] = -3*x[n - 3] + 12*x[n - 2] + 17*x[n - 1] + 12*((double)PCJ.get(p, Shared.right, 0)) - 3*((double)PCJ.get(p, Shared.right, 1));
            xprim[n - 1] = xprim[n - 1] * a;
        }

        // wypisanie krzywej oryginalnej i przefiltrowaniej
        for(int p = 0; p < PCJ.threadCount(); p++)
        {
            if(PCJ.myId() == p)
            {
                for(int i = 0; i < n; i++)
                {
                    System.out.println(PCJ.myId() + " " + (p*n + i) + " " + x[i] + " " + xprim[i]);
                }
            }
            PCJ.barrier();
        }
    } 
}
