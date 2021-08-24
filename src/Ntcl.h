#ifndef NTCL_H
#define NTCL_H

#include <iostream>
#include <tcl.h>

//
// Example class to catch stdout and stderr channel output.
//
// In the real world, this would be a GUI class (in Qt, KWWidgets etc)
// that makes the proper API calls to display the output in the right
// widget.
class TclIOCatcher
{
public:
  void outputText(const char *buf, int toWrite)
    {
      std::cout.write(buf,toWrite);
    }
};

//
// Tcl is pure C, and this is a C++ program; to ensure proper
// calling linkage, encapsulate callbacks in a extern "C" section.
extern "C"
{
  // outputproc is callback used by channel to handle data to outpu
  static int outputproc(ClientData instanceData,
                        CONST84 char *buf, int toWrite, int *errorCodePtr)
  {
    // instanceData in this case is a pointer to a class instance
    TclIOCatcher *qd =
      reinterpret_cast<TclIOCatcher *>(instanceData);
    qd->outputText(buf,toWrite);
    return toWrite;
  }
  // inputproc doesn't do anything in an output-only channel.
  static int
  inputproc(ClientData instancedata, char *buf, int toRead, int *errorCodePtr)
  {
    return TCL_ERROR;
  }
  // nothing to do on close
  static int
  closeproc(ClientData instancedata, Tcl_Interp *interp)
  {
    return 0;
  }
  // no options for this channel
  static int
  setoptionproc(ClientData instancedata, Tcl_Interp *interp,
                CONST84 char *optionname, CONST84 char *value)
  {
    return TCL_OK;
  }
  // for non-blocking I/O, callback when data is ready.
  static void
  watchproc(ClientData instancedata, int mask)
  {
    /* not much to do here */
    return;
  }
  // gethandleproc -- retrieves device-specific handle, not applicable here.
  static int
  gethandleproc(ClientData instancedata, int direction, ClientData *handlePtr)
  {
    return TCL_ERROR;
  }
  // Tcl Channel descriptor type.
  // many procs can be left NULL, and for our purposes
  // are left so.
  Tcl_ChannelType TclChan = {
    "tclIOTestChan",           /* typeName */
    TCL_CHANNEL_VERSION_5,      /* channel type version */
    closeproc,                  /* close proc */
    inputproc,                  /* input proc */
    outputproc,                 /* output proc */
    NULL,                       /* seek proc - can be null */
    setoptionproc,              /* set option proc - can be null */
    NULL,                       /* get option proc - can be null */
    watchproc,                  /* watch proc */
    gethandleproc,              /* get handle proc */
    NULL,                       /* close 2 proc - can be null */
    NULL,                       /* block mode proc - can be null */
    NULL,                       /* flush proc - can be null */
    NULL,                       /* handler proc - can be null */
    NULL,                       /* wide seek proc - can be null if seekproc is*/
    NULL                        /* thread action proc - can be null */
  };
}


int
Tcl_RegisterIO(Tcl_Interp *interp)
{
  // // create instance of the Tcl interpreter
  // Tcl_Interp *interp(Tcl_CreateInterp());
  // Tcl_Init(interp);

  // class object to catch output
  TclIOCatcher test;

  // create a new channel for stdout
  Tcl_Channel m_Out =
    Tcl_CreateChannel(&TclChan,
                      "testout",
                      &test,TCL_WRITABLE);
  //
  // IMPORTANT -- tcl Channels do buffering, so
  // the output catcher won't get called until a buffer
  // is filled (default 4K bytes).
  // These settings are stolen from TkWish.
  Tcl_SetChannelOption(NULL,m_Out,
                       "-translation", "lf");
  Tcl_SetChannelOption(NULL,m_Out,
                       "-buffering", "none");
  Tcl_SetChannelOption(NULL,m_Out,
                       "-encoding", "utf-8");
  //
  // make this new channel the standard output channel.
  Tcl_SetStdChannel(m_Out,TCL_STDOUT);
  //
  // I'm not sure why this is necessary, but apparently it has
  // something to do with how reference counting inside the interpeter works.
  Tcl_RegisterChannel(interp,m_Out);

  //
  // do all the same stuff for stderr.  In our case, we push the
  // output all to the same place, but you could handle it seperately.
  Tcl_Channel m_Err =
    Tcl_CreateChannel(&TclChan,
                      "testerr",
                      &test,TCL_WRITABLE);

  Tcl_SetChannelOption(NULL,m_Err,
                       "-translation", "lf");
  Tcl_SetChannelOption(NULL,m_Err,
                       "-buffering", "none");
  Tcl_SetChannelOption(NULL,m_Err,
                       "-encoding", "utf-8");

  Tcl_SetStdChannel(m_Err,TCL_STDERR);

  Tcl_RegisterChannel(interp,m_Err);


  

  //run one command to demonstrate how it works
  // const char testcommand[] = "puts $tcl_version";
  // int result = (interp,testcommand,strlen(testcommand),0);
  // // show the result, should be zero.
  // std::cout << "Result = " << result << std::endl;
  // exit(result);
  return 0;
}
#endif
