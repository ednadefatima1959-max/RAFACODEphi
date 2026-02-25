package com.termux.app;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.robolectric.RobolectricTestRunner;

@RunWith(RobolectricTestRunner.class)
public class BackgroundJobTest {

    @Test
    public void constructor_whenExecFails_sendsErrorResultAndNotifiesService() throws Exception {
        TermuxService service = mock(TermuxService.class);
        Context context = mock(Context.class);
        when(service.getApplicationContext()).thenReturn(context);

        PendingIntent pendingIntent = mock(PendingIntent.class);

        BackgroundJob job = new BackgroundJob(
                "/",
                "/path/that/does/not/exist/background-job.sh",
                null,
                service,
                pendingIntent
        );

        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(pendingIntent).send(eq(context), eq(Activity.RESULT_CANCELED), intentCaptor.capture());
        verify(service).onBackgroundJobExited(eq(job));

        Intent sentIntent = intentCaptor.getValue();
        assertNotNull(sentIntent);

        Bundle result = sentIntent.getBundleExtra("result");
        assertNotNull(result);
        assertEquals(-1, result.getInt("exitCode"));
        assertEquals("", result.getString("stdout"));

        String stderr = result.getString("stderr");
        assertNotNull(stderr);
        assertTrue(stderr.contains("Failed running background job:"));
    }
}
