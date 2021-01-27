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
package br.com.protowork.engine;

import br.com.protowork.util.ConstantsImageProc;


public class ImageProcessThread  extends Thread {
    
    private char typeService;
    
    private String videoFileName;
    private String regionParameters;
    private String configParameters;
    private String projectName;
    private String stringResultFromNativeCall;
    private boolean threadTerminated;
    
    private String databaseTrainingFilename;
    
    public static final char SERVICE_PROCESS_IMAGE                  = 'A';
    public static final char SERVICE_CREATE_REGION                  = 'B';
    public static final char SERVICE_PREVIEW_VIDEO                  = 'C';
    public static final char SERVICE_VISUALIZATION_IMAGE_WINDOW     = 'E';
    public static final char SERVICE_CREATE_DATABASE_TRAINNING      = 'D';
    public static final int INVALID_VALUE = -9999;
    
    static {
        /*
        try {
            LibraryUtil.addLibraryPath(ConstantsImageProc.OPENCV_LIBRARY_PATH);
        } catch (Exception ex) {
            throw new IllegalArgumentException("Biblioteca: " + ex.getMessage());
        }
        */
        System.load(ConstantsImageProc.DLL_LIB_PATH_IMAGE_PROC);
    }
    
    public ImageProcessThread(char typeService){
        this.typeService = typeService;
    }
    
    native private int processImage(String videoFileName, String projectName, String regionParameters, String configParametes);
    native private String createRegions(String filename, String configParameters);
    native private int previewRegions(String videoFileName, String regionParameters);
    native private long databaseTrainning(String videoFileName, String regionParameters, String databaseTrainingFilename, String configParameters);
    native private int showWindowProcessImage();
    
    @Override
    public void run(){
        switch(this.typeService){
            case ImageProcessThread.SERVICE_PROCESS_IMAGE:
                int ret = this.processImage(this.videoFileName, this.projectName, this.regionParameters, this.configParameters);
                this.setThreadTerminated(true);
                break;
            default:
        }
    }

    public boolean isThreadTerminated() {
        return threadTerminated;
    }

    public void setThreadTerminated(boolean threadTerminated) {
        this.threadTerminated = threadTerminated;
    }
    
    public void executeJavaWithoutThread_CreateRegion(){
        switch(this.typeService){
            case ImageProcessThread.SERVICE_CREATE_REGION:
                this.stringResultFromNativeCall = this.createRegions( this.videoFileName, this.configParameters);
                break;
            default:
        }
    }
    
    public int executeJavaWithoutThread_Preview(){
        int ret = ImageProcessThread.INVALID_VALUE;
        switch(this.typeService){
            case ImageProcessThread.SERVICE_PREVIEW_VIDEO:
                ret = this.previewRegions( this.videoFileName, this.regionParameters );
                break;
            default:
        }
        return ret;
    }
    
    
    public long executeJavaWithoutThread_DataBaseTrainning(){
        long ret = ImageProcessThread.INVALID_VALUE;
        switch(this.typeService){
            case ImageProcessThread.SERVICE_CREATE_DATABASE_TRAINNING:
                ret = this.databaseTrainning( this.videoFileName, this.regionParameters, this.databaseTrainingFilename, this.configParameters);
                break;
            default:
        }
        return ret;
    }
    
    public int executeJavaWithoutThread_showWindowProcessImage(){
        return this.showWindowProcessImage();
    }

    public String getConfigParameters() {
        return configParameters;
    }

    public void setConfigParameters(String configParameters) {
        this.configParameters = configParameters;
    }

    public String getProjectName() {
        return projectName;
    }

    public void setProjectName(String projectName) {
        this.projectName = projectName;
    }
    
    public String getVideoFileName() {
        return videoFileName;
    }

    public void setVideoFileName(String videoFileName) {
        this.videoFileName = videoFileName;
    }

    public String getRegionParameters() {
        return regionParameters;
    }

    public void setRegionParameters(String regionParameters) {
        this.regionParameters = regionParameters;
    }

    public String getStringResultFromNativeCall() {
        return stringResultFromNativeCall;
    }

    public void setStringResultFromNativeCall(String stringResultFromNativeCall) {
        this.stringResultFromNativeCall = stringResultFromNativeCall;
    }
    
    public String getDatabaseTrainingFilename() {
        return databaseTrainingFilename;
    }

    public void setDatabaseTrainingFilename(String databaseTrainingFilename) {
        this.databaseTrainingFilename = databaseTrainingFilename;
    }
    
}
