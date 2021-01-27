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
import br.com.protowork.util.ConstantsImageProc;
import br.com.protowork.util.FileUtil;
import br.com.protowork.vo.Project;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;
import javax.swing.JOptionPane;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import org.xml.sax.InputSource;

/**
 *
 * @author lucianav
 */
public class PresetXmlCreator {
    
    private List<Project> projectList = new ArrayList<>();
    
    public List<Project> getProjects() {
        return projectList;
    }
    
    public void addProject(Project project){
        if (this.projectList == null){
            this.projectList = new ArrayList<>(); 
        }
        this.projectList.add(project);
    }
    
    public void addProjectFirst(Project project){
        if (this.projectList == null){
            this.projectList = new ArrayList<>(); 
        }
        this.projectList.add(0, project);
    }

    public void setProjectList(List<Project> projects) {
        this.projectList = projects;
    }

    public void generateXML() {
        
        if (this.projectList == null){
            throw new IllegalArgumentException("lista de projetos nao pode ser vazia");
        }
        StringBuilder buffer = new StringBuilder();
        Project project = null;
        buffer.append("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        buffer.append("<presets>\n");

        for (int i = 0; i < this.projectList.size(); i++) {
            project = this.projectList.get(i);

            buffer.append("    <project id=\"").append(i + 1).append("\">\n");

            buffer.append("             <project_name>\n");
            buffer.append("                 ").append(project.getProjectName()).append("\n");
            buffer.append("            </project_name>\n");
            
            buffer.append("             <video_file>\n");
            buffer.append("                 ").append(project.getVideoFileName()).append("\n");
            buffer.append("            </video_file>\n");
            
            buffer.append("            <sensibility>\n");
            buffer.append("                 ").append(project.getSensibilityLevel()).append("\n");
            buffer.append("            </sensibility>\n");

            buffer.append("            <blur>\n");
            buffer.append("                 ").append(project.getBlurLevel()).append("\n");
            buffer.append("            </blur>\n");

            buffer.append("            <regions>\n");
            for (int k = 0; k < project.getListRegions().size(); k++) {
                MessageSelectedRegion region = project.getListRegions().get(k);
                
                buffer.append("                    <region index=\"").append(region.getIndexRegion()).append("\">\n");
                buffer.append("                            <px1>\n");
                buffer.append("                                 ").append(region.getP1x()).append("\n");
                buffer.append("                            </px1>\n");
                buffer.append("                            <py1>\n");
                buffer.append("                                 ").append(region.getP1y()).append("\n");
                buffer.append("                           </py1>\n");

                buffer.append("                            <px2>\n");
                buffer.append("                                 ").append(region.getP2x()).append("\n");
                buffer.append("                            </px2>\n");
                buffer.append("                            <py2>\n");
                buffer.append("                                 ").append(region.getP2y()).append("\n");
                buffer.append("                            </py2>\n");
                buffer.append("                    </region>\n");
            }
            buffer.append("            </regions>\n");
            
            buffer.append("    </project>\n");
        }

        buffer.append("</presets>\n");

        FileWriter writer = null;

        String filepath = ConstantsImageProc.PATH_XML_PRESET + ConstantsImageProc.XML_PRESET_FILENAME;
        try {
            
            FileUtil.createDirectory(ConstantsImageProc.PATH_XML_PRESET);
            writer = new FileWriter(new File(filepath));
            writer.write(buffer.toString());
            writer.close();
        } catch (IOException io) {
            JOptionPane.showMessageDialog(null, "Ocorreu um erro ao salvar o arquivo " + filepath + "\nVerifique as permissões de acesso ao diretório ou arquivo");
        }
    }
    
    public List<Project> loadXMLPreset() {
        
        String path = ConstantsImageProc.PATH_XML_PRESET + ConstantsImageProc.XML_PRESET_FILENAME;
        ParserXMLHandler parser = new ParserXMLHandler();
        List<Project> projetos = null;
        
        try {
        
        
            FileReader xmlfile = new FileReader(path);
            BufferedReader buffer = new BufferedReader(xmlfile);
            String xmlPartStr = null;
            StringBuilder builder = new StringBuilder();

            while ((xmlPartStr = buffer.readLine()) != null) {
                builder.append(xmlPartStr);
            }

            ParserXMLHandler handler = new ParserXMLHandler();
            SAXParserFactory factory = SAXParserFactory.newInstance();
            SAXParser saxParser = factory.newSAXParser();
            DocumentBuilderFactory factory1 = DocumentBuilderFactory.newInstance();
            DocumentBuilder db = factory1.newDocumentBuilder();
            InputSource inStream = new InputSource();

            inStream.setCharacterStream(new StringReader(builder.toString()));
            saxParser.parse(inStream, handler);

            projetos = handler.getListaProjetos();
            
            return projetos;
            
            
        } catch (Exception e){
            try {
                e.printStackTrace();
                throw e;
                
            } catch (Exception ex) {
                return null;
            }
        }
        
    }
}
