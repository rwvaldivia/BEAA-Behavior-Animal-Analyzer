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
package br.com.protowork.message.impl;

import java.io.Serializable;

public class MessageMember implements Serializable {

    // types of initial
    public static final char ZERO = '0',
            BLANK = ' ',
            NINE = '9';
    // type of supply
    public static final short RIGHT_BLANK = 0,
            RIGHT_ZERO = 1,
            LEFT_BLANK = 2,
            LEFT_ZERO = 3;
    private StringBuffer bufferStr;
    private short supplyString;
    private Character typeInitialField = null;

    public MessageMember(int capacity) {
        this(capacity, RIGHT_BLANK);
    }

    public MessageMember(int capacity, short supplyString) {
        bufferStr = new StringBuffer(capacity);
        this.supplyString = supplyString;
        initField();
    }

    // -- 
    public void initField() {
        switch (getSupplyString()) {
            case LEFT_BLANK:
            case RIGHT_BLANK:
                initField(BLANK);
                break;
            case LEFT_ZERO:
            case RIGHT_ZERO:
                initField(ZERO);
                break;
        }
    }

    // -- 
    public void initField(char typeInitial) {
        typeInitialField = new Character(typeInitial);

        for (int i = 0; i < bufferStr.capacity(); i++) {
            // Verify if buffer length is already the capacity. If yes, is not able to append
            //      char typeInitial in this buffer.
            if (bufferStr.length() < bufferStr.capacity()) {
                bufferStr.append(typeInitial);
            }
        }
    }

    // -- 
    public boolean setStringNum(String str) {
        try {
            return setString(String.valueOf((Integer.parseInt(str) * 100)));
        } catch (Exception ex) {
        }
        return false;
    }

    // --
    public boolean setString(String str) {
        String newStr = str;

        // somente fazer o trim se exceder a capacidade do campo
        // no caso do Objeto da Classe Name a capacidade sempre
        // sera maior que a capacidade. ver a classe Name.

        // nos demais casos nao realizar o trim, pois no caso do
        // Address pode apagar a informacao do Cep. ver a classe Address.

        if (newStr.length() > bufferStr.capacity()) {
            newStr = newStr.trim();
        }

        if (newStr.length() <= bufferStr.capacity()) {
            try {
                bufferStr.delete(0, bufferStr.toString().length());
                newStr = supplyString(newStr);
                bufferStr.append(newStr);
                return true;
            } catch (Exception ex) {
            }
        }
        return false;
    }

    // --
    public String getStringNum() {
        try {
            return String.valueOf((Integer.parseInt(toString()) / 100));
        } catch (Exception ex) {
        }
        return new String();
    }

    // --
    public String getString() {
        String ret = new String();
        try {
            if (getSupplyString() == LEFT_ZERO) {
                ret = String.valueOf((Integer.parseInt(toString())));
            } else {
                if (typeInitialField != null) {

                    MessageMember buff = new MessageMember(capacity(), getSupplyString());

                    buff.initField(typeInitialField.charValue());

                    if (toString().equals(buff.toString())) {
                        buff = new MessageMember(capacity(), getSupplyString());
                        buff.initField(BLANK);
                        ret = buff.toString();
                    } else {
                        ret = toString();
                    }
                } else {
                    ret = toString();
                }
            }
        } catch (Exception ex) {
        }
        return ret.trim();
    }

    // --
    public String toString() {
        return bufferStr.toString();
    }

    // --
    public int capacity() {
        return bufferStr.capacity();
    }

    // --
    public short getSupplyString() {
        return supplyString;
    }

    // --
    private String supplyString(String newStr) {
        if (newStr.length() < bufferStr.capacity()) {
            int diff = bufferStr.capacity() - newStr.length();
            switch (supplyString) {
                case RIGHT_BLANK:
                    for (int i = 0; i < diff; i++) {
                        newStr = newStr.concat(" ");
                    }
                    break;
                case RIGHT_ZERO:
                    for (int i = 0; i < diff; i++) {
                        newStr = newStr.concat("0");
                    }
                    break;
                case LEFT_BLANK:
                    String leftb = new String();
                    for (int i = 0; i < diff; i++) {
                        leftb = leftb.concat(" ");
                    }
                    newStr = leftb + newStr;
                    break;
                case LEFT_ZERO:
                    String leftz = new String();
                    for (int i = 0; i < diff; i++) {
                        leftz = leftz.concat("0");
                    }
                    newStr = leftz + newStr;
                    break;
            }
        }
        return newStr;
    }
}
