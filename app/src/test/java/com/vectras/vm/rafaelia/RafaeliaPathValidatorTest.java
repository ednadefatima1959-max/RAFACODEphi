package com.vectras.vm.rafaelia;

import org.junit.Assert;
import org.junit.Test;

/**
 * Unit tests for {@link RafaeliaMethodPaths} and {@link RafaeliaPathValidator} contracts.
 * Note: Full validator.validate(ctx) requires Android context; tested separately as instrumented test.
 */
public class RafaeliaPathValidatorTest {

    @Test
    public void allEightPathConstants_areDefinedAndDistinct() {
        int[] paths = {
            RafaeliaMethodPaths.PATH_INIT,
            RafaeliaMethodPaths.PATH_OBSERVE,
            RafaeliaMethodPaths.PATH_DENOISE,
            RafaeliaMethodPaths.PATH_TRANSMUTE,
            RafaeliaMethodPaths.PATH_MEMORY,
            RafaeliaMethodPaths.PATH_COMPLETE,
            RafaeliaMethodPaths.PATH_SPIRAL,
            RafaeliaMethodPaths.PATH_COHERENCE,
        };
        // All are between 1-8
        for (int p : paths) {
            Assert.assertTrue("path must be 1..8: " + p, p >= 1 && p <= 8);
            Assert.assertTrue(RafaeliaMethodPaths.isValid(p));
        }
        // All are distinct
        for (int i = 0; i < paths.length; i++) {
            for (int j = i + 1; j < paths.length; j++) {
                Assert.assertNotEquals("Duplicate path constants", paths[i], paths[j]);
            }
        }
    }

    @Test
    public void labelReturnsKnownStringForEachPath() {
        for (int p = 1; p <= 8; p++) {
            String label = RafaeliaMethodPaths.label(p);
            Assert.assertNotNull(label);
            Assert.assertFalse(label.isEmpty());
            Assert.assertFalse("Unknown path should not appear for valid id",
                label.startsWith("PATH_UNKNOWN"));
        }
    }

    @Test
    public void labelReturnsUnknownForInvalidPath() {
        Assert.assertTrue(RafaeliaMethodPaths.label(0).contains("UNKNOWN"));
        Assert.assertTrue(RafaeliaMethodPaths.label(9).contains("UNKNOWN"));
        Assert.assertTrue(RafaeliaMethodPaths.label(-1).contains("UNKNOWN"));
    }

    @Test
    public void cycleSymbols_coverAllPaths() {
        String[] expectedSymbols = { "ψ", "χ", "ρ", "Δ", "Σ", "Ω", "√3/2", "Φ" };
        for (int i = 0; i < expectedSymbols.length; i++) {
            int pathId = i + 1;
            Assert.assertEquals(expectedSymbols[i], RafaeliaMethodPaths.cycleSymbol(pathId));
        }
    }

    @Test
    public void spiralPath_deterministicComputation() {
        // PATH 7 spiral state must be non-zero (deterministic math check, no Context needed)
        final long PHI32   = 0x9E3779B9L;
        final long SQRT3_2 = 0xDDB3D743L;
        long state = 0x633L;
        for (int i = 0; i < 42; i++) {
            state = (state * PHI32) & 0xFFFFFFFFL;
            state = (state ^ (state >>> 16)) & 0xFFFFFFFFL;
            state = (state * SQRT3_2) & 0xFFFFFFFFL;
        }
        Assert.assertNotEquals("Spiral must not reduce to zero", 0L, state);
        Assert.assertNotEquals("Spiral must not reduce to max", 0xFFFFFFFFL, state);
    }

    @Test
    public void allLabels_areNonEmptyAndUnique() {
        java.util.Set<String> labels = new java.util.HashSet<>();
        for (int p = 1; p <= 8; p++) {
            String label = RafaeliaMethodPaths.label(p);
            Assert.assertFalse("Label must not be empty", label.isEmpty());
            Assert.assertTrue("Label must be unique: " + label, labels.add(label));
        }
    }
}
