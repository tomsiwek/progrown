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

@RegisterStorage(PCJSumowanieAsyn.Shared.class)

public class PCJSumowanieAsyn implements StartPoint
{
    double a;
    @Storage(PCJSumowanieAsyn.class)
    enum Shared {a};
    Scanner sc = new Scanner(System.in);
    
    public static void main(String args[]) throws IOException
    {
        PCJ.start(PCJSumowanieAsyn.class, new NodesDescription("nodes.txt"));
    }

    @Override
    public void main() throws Throwable
    {
       PcjFuture aL[] = new PcjFuture[PCJ.threadCount()];
       a = PCJ.myId();
       PCJ.barrier();
       double suma = 0;
       if(PCJ.myId() == 0)
       {
            for(int p = 0; p < PCJ.threadCount(); p++)
            {
                aL[p] = PCJ.asyncGet(p, Shared.a);
            }
       }
       PCJ.barrier();
       if(PCJ.myId() == 0)
       {
            for(int p = 0; p < PCJ.threadCount(); p++)
            {
                suma = suma + (double) aL[p].get();
            }
            System.out.println("suma = " + suma);
       }
    }
}
