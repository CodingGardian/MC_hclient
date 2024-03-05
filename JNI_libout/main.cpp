#include <include/jni.h>

int main()
{
    using namespace std;
    JavaVM* jvm;                      // Pointer to the JVM (Java Virtual Machine)
    JNIEnv* env;                      // Pointer to native interface
    //================== prepare loading of Java VM ============================
    JavaVMInitArgs vm_args;                        // Initialization arguments
    JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
    char string[20] = "-Djava.class.path=";
    options[0].optionString = string;   // where to find java .class
    vm_args.version = JNI_VERSION_1_6;             // minimum Java version
    vm_args.nOptions = 1;                          // number of options
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
    //=============== load and initialize Java VM and JNI interface =============
    jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);  // YES !!
    delete options;    // we then no longer need the initialisation options. 
    if (rc != JNI_OK) {
        // TO DO: error processing... 
        return 1;
    }
    //=============== Display JVM version =======================================
    //cout << "JVM load succeeded: Version ";
    jint ver = env->GetVersion();
    //cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << endl;

    // TO DO: add the code that will use JVM <============  (see next steps)

    jvm->DestroyJavaVM();
    //cin.g
    return 0;
}