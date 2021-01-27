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
package br.com.protowork.util;

import java.util.Date;

public class Util {
    public static String processTime(Date ini_dt, Date end_dt) {
        
        long diff           = end_dt.getTime() - ini_dt.getTime();
        long diffSeconds    = diff / 1000 % 60;
        long diffMinutes    = diff / (60 * 1000) % 60;
        long diffHours      = diff / (60 * 60 * 1000) % 24;
        long diffDays       = diff / (24 * 60 * 60 * 1000);

        //diffDays + " days
        return (Util.addZeroLeft(diffHours, 2) + ":" + Util.addZeroLeft(diffMinutes, 2) + ":" + Util.addZeroLeft(diffSeconds, 2) + " hs");
    }
    
    public static String addZeroLeft(long value, int size){
        
        String temp = String.valueOf(value);
        while (temp.length() < size){
            temp = "0" + temp;
        }
        return temp;
        
        
    }
    public static String addZeroLeft(int value, int size){
        long val = value;
        return Util.addZeroLeft(val, size);
    }
    
}
