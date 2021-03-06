// dev_stdout.cpp
//
// Send audio to standard output.
//
//   (C) Copyright 2014-2016 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <stdlib.h>
#include <unistd.h>

#include "dev_stdout.h"

DevStdout::DevStdout(unsigned pregap,Codec *codec,QObject *parent)
  : AudioDevice(pregap,codec,parent)
{
  stdout_format=AudioDevice::S16_LE;
}


DevStdout::~DevStdout()
{
}


bool DevStdout::processOptions(QString *err,const QStringList &keys,
			       const QStringList &values)
{
  for(int i=0;i<keys.size();i++) {
    bool processed=false;
    if(keys[i]=="--stdout-format") {
      if(values[i].toLower()=="float") {
	stdout_format=AudioDevice::FLOAT;
	processed=true;
      }
      if(values[i].toLower()=="s16_le") {
	stdout_format=AudioDevice::S16_LE;
	processed=true;
      }
      if(values[i].toLower()=="s32_le") {
	stdout_format=AudioDevice::S32_LE;
	processed=true;
      }
      if(!processed) {
	*err=tr("invalid --stdout-format value");
	return false;
      }
    }
    if(!processed) {
      *err=tr("unrecognized option")+" "+keys[i]+"\"";
      return false;
    }
  }
  return true;
}


bool DevStdout::start(QString *err)
{
  return true;
}


void DevStdout::synchronousWrite(unsigned frames,bool is_last)
{
  float pcm[frames*codec()->channels()];
  int16_t *pcm16;
  int32_t *pcm32;
  ssize_t blocksize=0;
  ssize_t n=0;
  
  codec()->ring()->read(pcm,frames);
  switch(stdout_format) {
  case AudioDevice::FLOAT:
    blocksize=frames*codec()->channels()*sizeof(float);
    n=write(1,pcm,blocksize);
    break;

  case AudioDevice::S16_LE:
    pcm16=new int16_t[frames*codec()->channels()];
    convertFromFloat(pcm16,pcm,frames,codec()->channels());
    blocksize=frames*codec()->channels()*sizeof(int16_t);
    n=write(1,pcm16,blocksize);
    delete pcm16;
    break;

  case AudioDevice::S32_LE:
    pcm32=new int32_t[frames*codec()->channels()];
    convertFromFloat(pcm32,pcm,frames,codec()->channels());
    blocksize=frames*codec()->channels()*sizeof(int32_t);
    n=write(1,pcm32,blocksize);
    delete pcm32;
    break;

  case AudioDevice::LastFormat:
    break;
  }
  if(n!=blocksize) {
    fprintf(stderr,"xrun: lost %ld bytes\n",blocksize-n);
  }
  if(is_last) {
    exit(0);
  }
}


void DevStdout::loadStats(QStringList *hdrs,QStringList *values,bool is_first)
{
  if(is_first) {
    hdrs->push_back("Device|Type");
    values->push_back("STDOUT");
  }
}
