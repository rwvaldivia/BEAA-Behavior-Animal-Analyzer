/*
#  BEAA - Behavior Animal Analyzer - Analisador de comportamento animal
#  Copyright 2019: Richard William Valdivia
#  E-mail: rwvaldivia@yahoo.com.br
#  
#  Este programa é um software livre; você pode redistribui-lo e/ou 
#  modifica-lo dentro dos termos da Licença Pública Geral GNU como 
#  publicada pela Fundação do Software Livre (FSF); na versão 2 da 
#  Licença, ou (na sua opinião) qualquer versão.
#
#  Este programa é distribuído na esperança que possa ser util, 
#  mas SEM NENHUMA GARANTIA; sem uma garantia implicita de ADEQUAÇÂO a qualquer
#  MERCADO ou APLICAÇÃO EM PARTICULAR. Veja a
#  Licença Pública Geral GNU para maiores detalhes.
#
#  Você deve ter recebido uma cópia da Licença Pública Geral GNU
#  junto com este programa, se não, escreva para a Fundação do Software
#  Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
*/
package br.com.protowork.thread;

import br.com.protowork.gui.ImageProcGUI;
import br.com.protowork.util.Util;
import java.util.Calendar;
import java.util.Date;
import javax.swing.JOptionPane;

public class ThreadObserver extends Thread {

    private DispatcherThreadPool dispatcher;
    private boolean terminateObserver = false;
    private final int THREAD_TIME_SLEEP_IN_MILLIS = 1000 * 3;
    private Date startTime = null;
    private Date endTime = null;

    @Override
    public void run() {
        
        while (!terminateObserver) {
            if (this.dispatcher != null) {
                
                if (this.startTime == null){
                    this.startTime = Calendar.getInstance().getTime();
                    this.dispatcher.dispatchThreadPool();
                }
                
                if (this.dispatcher.isPoolTerminated()) {
                    ImageProcGUI.threadChannelNotifier(this.dispatcher.getProjectId());
                    this.endTime = Calendar.getInstance().getTime();                    
                    JOptionPane.showMessageDialog(null, "Processamento terminado\nProjeto: " + dispatcher.getProjectName() + "\nTempo de Processamento: " + Util.processTime(startTime, endTime));
                    this.dispatcher = null;
                }
                
                try {
                    Thread.sleep(THREAD_TIME_SLEEP_IN_MILLIS);
                } catch (InterruptedException ex) {
                    this.dispatcher = null;
                    JOptionPane.showMessageDialog(null, "Erro de processamento no pool de threads: " + dispatcher.getProjectName());
                }
                
            } else {
                this.killObserver();
            }
        }
    }
    
    public boolean isTerminateObserver() {
        return terminateObserver;
    }
    
    public void killObserver() {
        this.terminateObserver = true;
    }
    
    public DispatcherThreadPool getDispatcher() {
        return dispatcher;
    }
    
    public void setDispatcher(DispatcherThreadPool dispatcher) {
        this.dispatcher = dispatcher;
    }
    
}
