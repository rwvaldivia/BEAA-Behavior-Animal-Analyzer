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
 * @author richardv
 */
public class MessageSelectedRegion extends MessageUtil {
    
    MessageMember indexRegion   = new MessageMember(10, MessageMember.RIGHT_ZERO);
    MessageMember p1x           = new MessageMember(10, MessageMember.RIGHT_ZERO);
    MessageMember p1y           = new MessageMember(10, MessageMember.RIGHT_ZERO);
    MessageMember p2x           = new MessageMember(10, MessageMember.RIGHT_ZERO);
    MessageMember p2y           = new MessageMember(10, MessageMember.RIGHT_ZERO);

    public String getIndexRegion() {
        return indexRegion.toString();
    }

    public void setIndexRegion(int indexRegion) {
        this.setIndexRegion(String.valueOf(indexRegion));
    }
    
    public void setIndexRegion(String indexRegion) {
        this.indexRegion.setString(indexRegion);
    }

    public String getP1x() {
        return p1x.toString();
    }

    public void setP1x(String p1x) {
        this.p1x.setString(p1x);
    }

    public String getP1y() {
        return p1y.toString();
    }

    public void setP1y(String p1y) {
        this.p1y.setString(p1y);
    }

    public String getP2x() {
        return p2x.toString();
    }

    public void setP2x(String p2x) {
        this.p2x.setString(p2x);
    }

    public String getP2y() {
        return p2y.toString();
    }

    public void setP2y(String p2y) {
        this.p2y.setString(p2y);
    }
}
