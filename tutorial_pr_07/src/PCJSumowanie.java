import java.io.IOException;
import java.util.Scanner;
import org.pcj.*;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author labszk
 */

@RegisterStorage(PCJSumowanie.Shared.class)

public class PCJSumowanie implements StartPoint
{
    double a;
    @Storage(PCJSumowanie.class)
    enum Shared {a};
    Scanner sc = new Scanner(System.in);
    
    public static void main(String args[]) throws IOException
    {
        PCJ.start(PCJSumowanie.class, new NodesDescription("nodes.txt"));
    }

    @Override
    public void main() throws Throwable
    {
        a = PCJ.myId();
        System.out.println("Thread no. " + PCJ.myId() + " a = " + a + ".");
        double suma = 0.0;
        if(PCJ.myId() == 0)
        {
            for(int p = 0; p < PCJ.threadCount(); p = p + 1)
            {
                suma = suma + (double) PCJ.get(p, Shared.a);
            }
            System.out.println("Suma = " + suma);
        }
    }
}
