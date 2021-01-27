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
package br.com.protowork.vo;

import br.com.protowork.message.MessageConfigProcessVideo;
import br.com.protowork.message.MessageSelectedRegion;
import java.util.ArrayList;
import java.util.List;

public class Project {
    private int threadGroupSession;
    private int idProject;
    private String projectName;
    private String videoFileName;
    private List<MessageSelectedRegion> listRegions = new ArrayList<>();
    private MessageConfigProcessVideo messageConfig = new MessageConfigProcessVideo();
    private int sensibilityLevel;
    private int blurLevel;
    private int brigthLevel;
    
    private int databaseTrainningProcessTime;
    private int databaseTrainningStep;
    private String databaseTrainingFilename;

    public int getDatabaseTrainningStep() {
        return databaseTrainningStep;
    }

    public void setDatabaseTrainningStep(int databaseTrainningStep) {
        this.databaseTrainningStep = databaseTrainningStep;
    }
    
    public int getDatabaseTrainningProcessTime() {
        return databaseTrainningProcessTime;
    }

    public void setDatabaseTrainningProcessTime(int databaseTrainningProcessTime) {
        this.databaseTrainningProcessTime = databaseTrainningProcessTime;
    }

    public String getDatabaseTrainingFilename() {
        return databaseTrainingFilename;
    }

    public void setDatabaseTrainingFilename(String databaseTrainingFilename) {
        this.databaseTrainingFilename = databaseTrainingFilename;
    }
    
    public int getThreadGroupSession() {
        return threadGroupSession;
    }

    public void setThreadGroupSession(int threadGroupSession) {
        this.threadGroupSession = threadGroupSession;
    }
    
    public int getIdProject() {
        return idProject;
    }

    public void setIdProject(int idProject) {
        this.idProject = idProject;
    }

    public MessageConfigProcessVideo getMessageConfig() {
        return messageConfig;
    }

    public void setMessageConfig(MessageConfigProcessVideo messageConfig) {
        this.messageConfig = messageConfig;
    }
    
    public String getMessageRegionsString() {
        String str = "";
        if (this.listRegions != null) {
            for (int i = 0; i < this.listRegions.size(); i++) {
                str += this.listRegions.get(i).toString();
            }
        }
        return str;
    }

    public String toMessageConfiguration(){
        this.messageConfig.setIdProject(String.valueOf(this.idProject));
        this.messageConfig.setBlurLevel(String.valueOf(this.blurLevel));
        this.messageConfig.setSensibilityLevel(String.valueOf(this.sensibilityLevel));
        this.messageConfig.setBrightnessLevel(String.valueOf(this.brigthLevel));
        this.messageConfig.setThreadGroupSession(String.valueOf(this.threadGroupSession));
        
        return this.messageConfig.toString();
        
    }
    public String getVideoFileName() {
        return videoFileName;
    }

    public void setVideoFileName(String videoFileName) {
        this.videoFileName = videoFileName;
    }

    public List<MessageSelectedRegion> getListRegions() {
        return listRegions;
    }

    public void setListRegions(List<MessageSelectedRegion> listRegions) {
        this.listRegions = listRegions;
    }

    public String getProjectName() {
        return projectName;
    }

    public void setProjectName(String projectName) {
        this.projectName = projectName;
    }

    public int getSensibilityLevel() {
        return sensibilityLevel;
    }

    public void setSensibilityLevel(int sensibilityLevel) {
        this.sensibilityLevel = sensibilityLevel;
    }

    public int getBlurLevel() {
        return blurLevel;
    }

    public void setBlurLevel(int blurLevel) {
        this.blurLevel = blurLevel;
    }

    public int getBrigthLevel() {
        return brigthLevel;
    }

    public void setBrigthLevel(int brigthLevel) {
        this.brigthLevel = brigthLevel;
    }
}
