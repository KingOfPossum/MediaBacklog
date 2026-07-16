<script setup lang="js">
import {load_library} from "~/index.js";
import {games} from "~/data/gameData.ts";

//games.value = [];

definePageMeta({
  layout:'main-layout'
})

//load_library()
</script>

<template>
  <GameView />
</template>
