package org.quercuscore.qt;

import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;

import org.qtproject.qt5.android.bindings.QtActivity;

import java.io.File;

public class QuercusQtActivity extends QtActivity
{
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        final File quercusDir = new File(getFilesDir().getAbsolutePath() + "/.quercus");
        if (!quercusDir.exists()) {
            quercusDir.mkdir();
        }

        super.onCreate(savedInstanceState);
    }
}
