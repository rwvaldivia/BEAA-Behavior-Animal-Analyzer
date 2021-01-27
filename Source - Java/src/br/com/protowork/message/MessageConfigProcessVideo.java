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
package br.com.protowork.message;

import br.com.protowork.message.impl.MessageMember;

/**
 *
 * @author Richardv
 */
public class MessageConfigProcessVideo extends MessageUtil {
    
    
    public MessageConfigProcessVideo(){}
    
    MessageMember threadGroupSession            = new MessageMember(10, MessageMember.LEFT_ZERO);
    MessageMember idProject                     = new MessageMember(10, MessageMember.LEFT_ZERO);
    MessageMember brightnessLevel               = new MessageMember(10, MessageMember.LEFT_ZERO); 
    MessageMember sensibilityLevel              = new MessageMember(10, MessageMember.LEFT_ZERO); 
    MessageMember blurLevel                     = new MessageMember(10, MessageMember.LEFT_ZERO);
    MessageMember sizeWidthPresetSelection      = new MessageMember(10, MessageMember.LEFT_ZERO);
    MessageMember sizeHeightPresetSelection     = new MessageMember(10, MessageMember.LEFT_ZERO);
    MessageMember typeBoxSelection              = new MessageMember(1, MessageMember.LEFT_ZERO);
    //#####################################
    //Usado para controle da base de treinamento
    MessageMember databaseTrainningProcessTime  = new MessageMember(10, MessageMember.LEFT_ZERO);
    MessageMember databaseTrainningProcessStep  = new MessageMember(10, MessageMember.LEFT_ZERO);

    public String getDatabaseTrainningProcessStep() {
        return databaseTrainningProcessStep.toString();
    }

    public void setDatabaseTrainningProcessStep(String databaseTrainningProcessStep) {
        this.databaseTrainningProcessStep.setString(databaseTrainningProcessStep);
    }
    
    public String getDatabaseTrainningProcessTime() {
        return databaseTrainningProcessTime.toString();
    }

    public void setDatabaseTrainningProcessTime(String databaseTrainningProcessTime) {
        this.databaseTrainningProcessTime.setString(databaseTrainningProcessTime);
    }
    
    public String getSizeWidthPresetSelection() {
        return sizeWidthPresetSelection.toString();
    }

    public void setSizeWidthPresetSelection(String sizeWidthPresetSelection) {
        this.sizeWidthPresetSelection.setString(sizeWidthPresetSelection);
    }

    public String getSizeHeightPresetSelection() {
        return sizeHeightPresetSelection.toString();
    }

    public void setSizeHeightPresetSelection(String sizeHeightPresetSelection) {
        this.sizeHeightPresetSelection.setString(sizeHeightPresetSelection);
    }
    
    public String getTypeBoxSelection() {
        return typeBoxSelection.toString();
    }

    public void setTypeBoxSelection(String typeBoxSelection) {
        this.typeBoxSelection.setString(typeBoxSelection);
    }
    
    public String getThreadGroupSession() {
        return threadGroupSession.toString();
    }
    
    public void setThreadGroupSession(String threadGroupSession) {
        this.threadGroupSession.setString(threadGroupSession);
    }
    
    public String getIdProject() {
        return idProject.toString();
    }

    public void setIdProject(String projectId) {
        this.idProject.setString(projectId);
    }
    
    public String getBrightnessLevel() {
        return brightnessLevel.toString();
    }

    public void setBrightnessLevel(String brightness) {
        this.brightnessLevel.setString(brightness);
    }

    public String getSensibilityLevel() {
        return sensibilityLevel.toString();
    }

    public void setSensibilityLevel(String sensibilityValue) {
        this.sensibilityLevel.setString(sensibilityValue);
    }

    public String getBlurLevel() {
        return blurLevel.toString();
    }

    public void setBlurLevel(String blurValue) {
        this.blurLevel.setString(blurValue);
    }
   
}
