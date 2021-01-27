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

import br.com.protowork.message.MessageConfigProcessVideo;
import br.com.protowork.message.MessageSelectedRegion;
import br.com.protowork.thread.DispatcherThreadPool;
import br.com.protowork.thread.ThreadObserver;
import br.com.protowork.util.ConstantsImageProc;
import br.com.protowork.util.FileUtil;
import br.com.protowork.vo.Project;
import java.util.ArrayList;
import java.util.List;


public class ImageProcess {
    
    public long processDatabaseTrainning(Project project){
        String videofilename    = project.getVideoFileName();
        String params           = project.getMessageRegionsString();
        ImageProcessThread engine = new ImageProcessThread(ImageProcessThread.SERVICE_CREATE_DATABASE_TRAINNING);
        engine.setProjectName(project.getProjectName());
        engine.setVideoFileName(videofilename);
        engine.setRegionParameters(params);
        
        MessageConfigProcessVideo config = new MessageConfigProcessVideo();
        config.setDatabaseTrainningProcessTime(String.valueOf(project.getDatabaseTrainningProcessTime()));
        config.setDatabaseTrainningProcessStep(String.valueOf(project.getDatabaseTrainningStep()));
        
        engine.setConfigParameters(config.toString());
        engine.setDatabaseTrainingFilename(project.getDatabaseTrainingFilename());
        
        return engine.executeJavaWithoutThread_DataBaseTrainning();
        
    }
    
    
    public void processSelectedProject(Project project){

        String videofilename    = project.getVideoFileName();
        String params           = project.getMessageRegionsString();
        
        //params = project.getListRegions().get(0).toString();
        //###############################
        //Cria o diretorio para o projeto
        FileUtil.createDirectory(ConstantsImageProc.PATH_PROJECT + "/" + project.getProjectName());
        
        ThreadObserver observer = new ThreadObserver();
        DispatcherThreadPool poolOfThreads = new DispatcherThreadPool(project.getThreadGroupSession(), project.getProjectName(), project.getIdProject());
        observer.setDispatcher(poolOfThreads);
        
        for (int i = 0; i < project.getListRegions().size(); i++) {
            
            ImageProcessThread engine = new ImageProcessThread(ImageProcessThread.SERVICE_PROCESS_IMAGE);
            engine.setProjectName(project.getProjectName());
            engine.setVideoFileName(videofilename);
            
            params = project.getListRegions().get(i).toString();
            
            engine.setRegionParameters(params);
            engine.setConfigParameters(project.toMessageConfiguration());
            
            poolOfThreads.addImageProcessThreadObject(engine);
            
            //engine.start();
        }
        
        observer.start();
        
    }
    
    public int showWindowProcessImage(){
        return new ImageProcessThread(ImageProcessThread.SERVICE_VISUALIZATION_IMAGE_WINDOW).executeJavaWithoutThread_showWindowProcessImage();        
    }
    
    public void previewVideo(Project project){
        
        ImageProcessThread engine = new ImageProcessThread(ImageProcessThread.SERVICE_PREVIEW_VIDEO);
        
        engine.setVideoFileName(project.getVideoFileName());
        engine.setRegionParameters(project.getMessageRegionsString());
        
        engine.executeJavaWithoutThread_Preview();
    }
    
    
    
    public List<MessageSelectedRegion> selectRegions(String filename, String typeBoxSelection, int width, int height){
        
        ImageProcessThread engine = new ImageProcessThread(ImageProcessThread.SERVICE_CREATE_REGION);
        engine.setVideoFileName(filename);
        
        MessageConfigProcessVideo config = new MessageConfigProcessVideo();
        config.setTypeBoxSelection(typeBoxSelection);
        config.setSizeHeightPresetSelection(String.valueOf(height));
        config.setSizeWidthPresetSelection(String.valueOf(width));
        
        engine.setConfigParameters(config.toString());
        
        engine.setRegionParameters(null);
        engine.executeJavaWithoutThread_CreateRegion();
        String result = engine.getStringResultFromNativeCall();
        int sizeElement = new MessageSelectedRegion().sizeOfFields();
        
        List<MessageSelectedRegion> listRegions = this.parseMessageReturned(result, sizeElement);
        return listRegions;
        
    }
    
    private List<MessageSelectedRegion> parseMessageReturned(String str, int size){
        
        List<MessageSelectedRegion> list   = new ArrayList<>();
        StringBuilder builder       = new StringBuilder();
        MessageSelectedRegion m_temp       = null;
        int loop                    = str.length() / size;
        int start                   = 0;
        int end                     = size;
        String temp                 = "";
        
        builder.append(str);
        
        for (int i = 0; i < loop; i++) {
            
            temp = builder.substring(start, end);
            
            m_temp = new MessageSelectedRegion();
            m_temp.parse(temp);
            list.add(m_temp);
            
            start   = end;
            end    += size;
            
        }
        
        return list;
    }
    
}
