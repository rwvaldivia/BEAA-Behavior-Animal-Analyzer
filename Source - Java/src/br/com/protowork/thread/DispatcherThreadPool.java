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

import br.com.protowork.engine.ImageProcessThread;
import java.util.ArrayList;
import java.util.List;

public class DispatcherThreadPool {
    
    private int threadGroupSession;
    private String projectName;
    private int projectId;
    private List<ImageProcessThread> threadList;

    public DispatcherThreadPool(int threadGroupSession, String projectName, int projectId){
        this.threadGroupSession     = threadGroupSession;
        this.projectName            = projectName;
        this.projectId              = projectId;
    }

    public int getThreadGroupSession() {
        return threadGroupSession;
    }

    public String getProjectName() {
        return projectName;
    }
    
    public void addImageProcessThreadObject(ImageProcessThread obj){
        if (this.threadList == null){
            this.threadList = new ArrayList<>();
        }
        this.threadList.add(obj);
    }
    
    public boolean isPoolTerminated(){
        int threadTerminatedCounter = 0;
        
        for (int i = 0; i < this.threadList.size(); i ++){
            if (this.threadList.get(i).isThreadTerminated()){
                ++threadTerminatedCounter;
            } else {
                break;
            }
        }
        return threadTerminatedCounter == this.threadList.size() ? true : false;
    }
    
    protected void dispatchThreadPool(){
        for (int i = 0; i < this.threadList.size(); i ++){
            this.threadList.get(i).start();
        }
    }
    
    public int getProjectId() {
        return projectId;
    }
    
}
