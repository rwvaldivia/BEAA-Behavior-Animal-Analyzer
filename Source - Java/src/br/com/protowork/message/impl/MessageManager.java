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

import java.io.*;
import java.lang.reflect.*;

public abstract class MessageManager implements Serializable {

    public MessageMember get(java.lang.reflect.Field field) throws IllegalAccessException {
        return (MessageMember) field.get(this);
    }

    public abstract void set(java.lang.reflect.Field field, MessageMember bs) throws IllegalAccessException ;

    public abstract Object getObject(java.lang.reflect.Field field) throws IllegalAccessException;
    
    public boolean parse(String str) {
        try {
            if (str.length() == sizeOfFields()) {
                Field[] fields = this.getClass().getDeclaredFields();
                Object obj;

                int posic = 0;

                for (int i = 0; i < fields.length; i++) {
                    obj = getObject(fields[i]);
                    if (obj instanceof MessageManager) {
                        MessageManager dds = (MessageManager) obj;
                        dds.parse(str.substring(posic, posic += dds.sizeOfFields()));
                    } else if (obj instanceof MessageMember) {
                        MessageMember bs = (MessageMember) obj;
                        bs.setString(str.substring(posic, posic += bs.capacity()));
                    }

                }

                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public int numberOfFields() {
        int result = 0;
        try {
            result = this.getClass().getDeclaredFields().length;
        } catch (Exception e) {
            e.printStackTrace();
        }
        return result;
    }

    public int getFieldSize(Field field) {
        int size = 0;
        try {
            Object obj = getObject(field);

            if (obj instanceof MessageManager) {
                size = ((MessageManager) obj).sizeOfFields();
            } else if (obj instanceof MessageMember) {
                size = ((MessageMember) obj).capacity();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return size;
    }

    public int getFieldSize(String fieldName) {
        int size = 0;
        try {
            return getFieldSize(this.getClass().getDeclaredField(fieldName));
        } catch (Exception e) {
            e.printStackTrace();
        }
        return size;
    }

    public int getFieldSize(int pos) {
        int size = 0;
        try {
            Field[] fields = this.getClass().getDeclaredFields();
            return getFieldSize(fields[pos]);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return size;
    }

    public int sizeOfFields() {
        int size = 0;
        try {
            Field[] fields = this.getClass().getDeclaredFields();

            for (int i = 0; i < fields.length; i++) {
                size += getFieldSize(fields[i]);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return size;
    }

    public String toString() {
        StringBuffer sb = new StringBuffer(0);
        try {
            Field[] fields = this.getClass().getDeclaredFields();
            for (int i = 0; i < fields.length; i++) {
                sb.append(getObject(fields[i]).toString());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return sb.toString();
    }

    public String allFieldsAndContent() {
        try {
            if (Boolean.getBoolean("cetelem.debugdatadict")) {
                return allFieldsAndContent("");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    private String allFieldsAndContent(String level) {
        StringBuilder sb = new StringBuilder(0);
        try {
            Field[] fields = this.getClass().getDeclaredFields();
            Object obj;

            for (int i = 0; i < fields.length; i++) {
                obj = getObject(fields[i]);

                if (!level.equals("")) {
                    sb.append(level);
                }

                sb.append(i).append(" ").append(fields[i].getName()).append(" = ");

                if (obj instanceof MessageManager) {
                    sb.append("\n").append(((MessageManager) obj).allFieldsAndContent(level + i + " "));
                } else {
                    sb.append(obj.toString()).append("\n");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return sb.toString();
    }

    @Override
    public boolean equals(Object obj) {
        return ((obj != null) && (obj.toString().equals(this.toString())));
    }

    public boolean setValueField(Field field, String value) {
        try {
            Object obj = getObject(field);

            if (obj instanceof MessageManager) {
                return ((MessageManager) obj).parse(value);
            } else if (obj instanceof MessageMember) {
                return ((MessageMember) obj).setString(value);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean setValueField(String nameField, String value) {
        try {
            return setValueField(this.getClass().getDeclaredField(nameField), value);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean setValueField(int pos, String value) {
        try {
            Field[] fields = this.getClass().getDeclaredFields();
            return setValueField(fields[pos], value);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public String getValueField(Field field) {
        try {
            return getObject(field).toString();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public String getValueField(String nameField) {
        try {
            return getValueField(this.getClass().getDeclaredField(nameField));
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public String getValueField(int pos) {
        try {
            Field[] fields = this.getClass().getDeclaredFields();
            return getValueField(fields[pos]);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public String getFieldName(int i) {
        try {
            Field[] fields = this.getClass().getDeclaredFields();
            return fields[i].getName();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return "";
    }

    public boolean setValueFieldSum(Field field, String value) {
        try {
            Object obj = getObject(field);
            if (obj instanceof MessageMember) {
                return ((MessageMember) obj).setStringNum(value);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean setValueFieldSum(String nameField, String value) {
        try {
            return setValueFieldSum(this.getClass().getDeclaredField(nameField), value);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }

    public boolean setValueFieldSum(int pos, String value) {
        try {
            Field[] fields = this.getClass().getDeclaredFields();
            return setValueFieldSum(fields[pos], value);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }
}
