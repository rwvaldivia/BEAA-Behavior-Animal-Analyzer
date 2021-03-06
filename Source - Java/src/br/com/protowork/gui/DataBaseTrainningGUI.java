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
package br.com.protowork.gui;

import br.com.protowork.engine.ImageProcess;
import br.com.protowork.thread.ThreadUtil;
import br.com.protowork.util.StringUtil;
import br.com.protowork.vo.Project;
import br.com.protowork.xml.PresetXmlCreator;
import java.util.List;
import javax.swing.JOptionPane;
import javax.swing.table.DefaultTableModel;

public class DataBaseTrainningGUI extends javax.swing.JDialog {

    /**
     * Creates new form DataBaseTrainningGUI
     */
    public DataBaseTrainningGUI(java.awt.Frame parent, boolean modal) {
        super(parent, modal);
        initComponents();
        
        PresetXmlCreator xml = new PresetXmlCreator();
        this.updateProjectsTable(xml.loadXMLPreset());
    }
    
    public void updateProjectsTable(List<Project> projetos){
        
        if (projetos == null){
            return;
        }
        
        DefaultTableModel model = (DefaultTableModel)this.tbProjetos.getModel();
        model.setRowCount(0);

        //String [] row = new String[projetos.size()];

        for (int i = 0 ; i < projetos.size(); i++){
            String [] row = new String[1];
            row[0] = projetos.get(i).getProjectName();
            model.addRow(row);
        }
        
        this.tbProjetos.setModel(model);
        
        model.fireTableDataChanged();
    }
    
    
    

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLabel2 = new javax.swing.JLabel();
        lblNomeAquivo = new javax.swing.JLabel();
        txtFileName = new javax.swing.JTextField();
        jScrollPane1 = new javax.swing.JScrollPane();
        tbProjetos = new javax.swing.JTable();
        btnProcessDataBaseTrainning = new javax.swing.JButton();
        btnSair = new javax.swing.JButton();
        btnPreview = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        txtSegundosDeExecucao = new javax.swing.JTextField();
        jLabel3 = new javax.swing.JLabel();
        txtStep = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);

        jLabel2.setFont(new java.awt.Font("Tahoma", 1, 18)); // NOI18N
        jLabel2.setText("Base de Treinamento");

        lblNomeAquivo.setText("Nome do Arquivo:");

        txtFileName.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                txtFileNameActionPerformed(evt);
            }
        });

        tbProjetos.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {
                {null},
                {null},
                {null},
                {null}
            },
            new String [] {
                "Projetos"
            }
        ) {
            boolean[] canEdit = new boolean [] {
                false
            };

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jScrollPane1.setViewportView(tbProjetos);

        btnProcessDataBaseTrainning.setText("Processar Base de Treinamento");
        btnProcessDataBaseTrainning.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnProcessDataBaseTrainningActionPerformed(evt);
            }
        });

        btnSair.setText("Sair");
        btnSair.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnSairActionPerformed(evt);
            }
        });

        btnPreview.setText("Preview");
        btnPreview.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btnPreviewActionPerformed(evt);
            }
        });

        jLabel1.setText("Tempo Proc. (s):");

        jLabel3.setText("Salto:");

        txtStep.setText("0");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                            .addComponent(jScrollPane1, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 408, Short.MAX_VALUE)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(btnProcessDataBaseTrainning)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(btnPreview, javax.swing.GroupLayout.PREFERRED_SIZE, 102, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(btnSair, javax.swing.GroupLayout.PREFERRED_SIZE, 99, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(javax.swing.GroupLayout.Alignment.LEADING, layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                                    .addComponent(lblNomeAquivo, javax.swing.GroupLayout.PREFERRED_SIZE, 97, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(jLabel1, javax.swing.GroupLayout.DEFAULT_SIZE, 103, Short.MAX_VALUE))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(layout.createSequentialGroup()
                                        .addComponent(txtSegundosDeExecucao, javax.swing.GroupLayout.PREFERRED_SIZE, 81, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(jLabel3, javax.swing.GroupLayout.PREFERRED_SIZE, 40, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                        .addComponent(txtStep, javax.swing.GroupLayout.PREFERRED_SIZE, 65, javax.swing.GroupLayout.PREFERRED_SIZE)
                                        .addGap(0, 0, Short.MAX_VALUE))
                                    .addComponent(txtFileName)))))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(100, 100, 100)
                        .addComponent(jLabel2, javax.swing.GroupLayout.PREFERRED_SIZE, 212, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addGap(26, 26, 26)
                .addComponent(jLabel2)
                .addGap(27, 27, 27)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lblNomeAquivo)
                    .addComponent(txtFileName, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(13, 13, 13)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(txtSegundosDeExecucao, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel3)
                    .addComponent(txtStep, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 105, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(42, 42, 42)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(btnProcessDataBaseTrainning)
                    .addComponent(btnSair)
                    .addComponent(btnPreview))
                .addContainerGap(18, Short.MAX_VALUE))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void btnSairActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnSairActionPerformed
        // TODO add your handling code here:
        /*
        if (ImageProcGUI.listProjectInProcessament.size() > 0){
            String msg = ""
                    + "Existem processos ainda em execução\n"
                    + "Para finalizar os processos siga os seguintes passos:\n\n"
                    + "1 - Clique no botão \"Visualizar Processamento\"\n"
                    + "2 - Pressione ESC em cada janela de procesamento e aguarde a finalização";
            JOptionPane.showMessageDialog(null, msg);
            return;
        }
        * */
        
        this.setVisible(false);
        this.dispose();
    }//GEN-LAST:event_btnSairActionPerformed

    private void btnPreviewActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnPreviewActionPerformed
        // TODO add your handling code here:
        
         int selectedRowIndex = this.tbProjetos.getSelectedRow();
        
        if (selectedRowIndex == -1){
            JOptionPane.showMessageDialog(null, "Selecione um projeto na tabela para realizar o preview");
            return;
        }
        
        List<Project> projects = new PresetXmlCreator().loadXMLPreset();
        Project proj = projects.get(selectedRowIndex);
        ImageProcess p = new ImageProcess();
        
        p.previewVideo(proj);
        
    }//GEN-LAST:event_btnPreviewActionPerformed

    private void txtFileNameActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_txtFileNameActionPerformed
        // TODO add your handling code here:
    }//GEN-LAST:event_txtFileNameActionPerformed

    private void btnProcessDataBaseTrainningActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btnProcessDataBaseTrainningActionPerformed
        // TODO add your handling code here:
        
        int selectedRowIndex = this.tbProjetos.getSelectedRow();
        
        if (selectedRowIndex == -1){
            JOptionPane.showMessageDialog(null, "Selecione um projeto na tabela de Presets");
            return;
        }
        
        String strTempProcessTime = StringUtil.letOnlyNumber(txtSegundosDeExecucao.getText());
        this.txtSegundosDeExecucao.setText(strTempProcessTime);
        
        if ("".equals(strTempProcessTime)){
            JOptionPane.showMessageDialog(null, "Preencha o tempo em segundos");
            return;
        }
        
        String strTempNumberStep = StringUtil.letOnlyNumber(this.txtStep.getText());
        
        if ("".equals(strTempNumberStep)){
            strTempNumberStep = "1";
        }
            
        this.txtStep.setText(strTempNumberStep);
        
        String filenameDatabase = this.txtFileName.getText().trim();
        
        if ("".equals(this.txtFileName.getText())){
            JOptionPane.showMessageDialog(null, "Preencha o nome do arquivo que será gerado");
            return;
        }
        
        if (filenameDatabase.indexOf(".") < 0){
            filenameDatabase += ".txt";
        }
        
        List<Project> projects = new PresetXmlCreator().loadXMLPreset();
        Project proj = projects.get(selectedRowIndex);
        
        proj.setDatabaseTrainningProcessTime(Integer.parseInt(strTempProcessTime));
        proj.setDatabaseTrainningStep(Integer.parseInt(strTempNumberStep));
        
        proj.setDatabaseTrainingFilename(filenameDatabase);
        
        
        ImageProcess p = new ImageProcess();
        long ret = p.processDatabaseTrainning(proj);
        if (ret > 0){
            JOptionPane.showMessageDialog(null, "Base de treinamento criada.\nTotal de Frames Processados: " + ret);
        } else {
            JOptionPane.showMessageDialog(null, "ERRO AO CRIAR Base de treinamento");
        }
        
        
    }//GEN-LAST:event_btnProcessDataBaseTrainningActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {
        /* Set the Nimbus look and feel */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /* If Nimbus (introduced in Java SE 6) is not available, stay with the default look and feel.
         * For details see http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html 
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(DataBaseTrainningGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(DataBaseTrainningGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(DataBaseTrainningGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(DataBaseTrainningGUI.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /* Create and display the dialog */
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                DataBaseTrainningGUI dialog = new DataBaseTrainningGUI(new javax.swing.JFrame(), true);
                dialog.addWindowListener(new java.awt.event.WindowAdapter() {
                    @Override
                    public void windowClosing(java.awt.event.WindowEvent e) {
                        System.exit(0);
                    }
                });
                dialog.setVisible(true);
            }
        });
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btnPreview;
    private javax.swing.JButton btnProcessDataBaseTrainning;
    private javax.swing.JButton btnSair;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JLabel lblNomeAquivo;
    private javax.swing.JTable tbProjetos;
    private javax.swing.JTextField txtFileName;
    private javax.swing.JTextField txtSegundosDeExecucao;
    private javax.swing.JTextField txtStep;
    // End of variables declaration//GEN-END:variables
}
