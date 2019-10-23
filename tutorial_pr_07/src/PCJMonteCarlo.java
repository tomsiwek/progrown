import java.io.IOException;
import java.util.Random;
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
@RegisterStorage(PCJMonteCarlo.Shared.class)

public class PCJMonteCarlo implements StartPoint
{
    @Storage(PCJMonteCarlo.class)
    enum Shared {s};
    long s = 0;
    
    public static void main (String args[]) throws IOException
    {
        PCJ.start(PCJMonteCarlo.class, new NodesDescription("nodes.txt"));
    }
    
    @Override
    public void main() throws Throwable
    {
        long nAll = 1000000000;
        long n = nAll / PCJ.threadCount();
        Random r = new Random();
        for(int i = 0; i < n; i++)
        {
            double x = 2*r.nextDouble()- 1;
            double y = 2*r.nextDouble() - 1;
            if(x*x + y*y <= 1) s = s + 1;
        }
        PCJ.barrier();
        long suma = 0;
        if(PCJ.myId() == 0)
        {
            for(int p = 0; p < PCJ.threadCount(); p++)
            {
                suma = suma + (long) PCJ.get(p, Shared.s);
            }
            System.out.println("pi ~= " + 4.0 * (double) suma / (double) nAll);
        }
    }
}
