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
package br.com.protowork.xml;

import br.com.protowork.message.MessageSelectedRegion;
import br.com.protowork.vo.Project;
import java.util.ArrayList;
import java.util.List;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

/**
 *
 * @author lucianav
 */
public class ParserXMLHandler extends DefaultHandler {
    private List<Project> listaProjetos;
    private Project projectTemp = null;
    
    private List<MessageSelectedRegion> listaMessageRegions;
    MessageSelectedRegion messageRegTemp   = null;
    
    String tmpValue             = null;
    
    public ParserXMLHandler(){
        this.listaProjetos = new ArrayList<>();
    }
    
    public int parseInt(String s){
        int value = 0;
        try {
            value = Integer.parseInt(s);
        } catch(Exception e){}
        return value;
    }

    public List<Project> getListaProjetos() {
        return listaProjetos;
    }

    public void setListaProjetos(List<Project> listaProjetos) {
        this.listaProjetos = listaProjetos;
    }
    
    @Override
    public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException {
        
        if (qName.equalsIgnoreCase("project")){
            
            this.projectTemp = new Project();
            this.listaMessageRegions = new ArrayList<>();
            
            int idProject = 0;
            
            try{
                idProject = Integer.parseInt(attributes.getValue("id"));
            } catch(Exception e){}
            
            this.projectTemp.setIdProject(idProject);
            this.projectTemp.setListRegions(listaMessageRegions);
            
            
            
            this.listaProjetos.add(projectTemp);
        
        } else if (qName.equalsIgnoreCase("region")){
            this.messageRegTemp = new MessageSelectedRegion();
            this.messageRegTemp.setIndexRegion(attributes.getValue("index"));
            this.listaMessageRegions.add(messageRegTemp);
        } 
        
    }
    
    
    @Override
    public void characters(char[] ch, int start, int length) throws SAXException {
        this.tmpValue = new String(ch, start, length).trim();
    }
    
    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        if (qName.equalsIgnoreCase("project_name")){
            
            this.projectTemp.setProjectName(this.tmpValue);
            
        } else if (qName.equalsIgnoreCase("video_file")){
            
            this.projectTemp.setVideoFileName(this.tmpValue);
            
        } else if (qName.equalsIgnoreCase("sensibility")){
            
            this.projectTemp.setSensibilityLevel(this.parseInt(this.tmpValue));
            
        } else if (qName.equalsIgnoreCase("blur")){
            
            this.projectTemp.setBlurLevel(this.parseInt(this.tmpValue));
            
        } else if (qName.equalsIgnoreCase("px1")){
            
            this.messageRegTemp.setP1x(this.tmpValue);
            
        } else if (qName.equalsIgnoreCase("py1")){
            
            this.messageRegTemp.setP1y(this.tmpValue);
              
        } else if (qName.equalsIgnoreCase("px2")){
            
            this.messageRegTemp.setP2x(this.tmpValue);
            
        } else if (qName.equalsIgnoreCase("py2")){
            
            this.messageRegTemp.setP2y(this.tmpValue);
            
        }
    }
    
}
