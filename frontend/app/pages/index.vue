<script setup lang="js">
import {load_library} from "~/index.js";

definePageMeta({
  layout:'main-layout'
})

load_library()
</script>

<template>
  <GameView />
</template>
