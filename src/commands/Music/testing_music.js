const chnl = require("../../../configs/chnl.json");
module.exports = {
  config: {
    name: `testing_music_play`,
    category: "",
    description: "",
    aliases: ["t_about"],
  },
  run: async (bot, message, args) => {
    try {
      
    } catch (err) {
      console.error(err);
      bot.channels.cache.get(chnl["pre-bed-errors"]).send(err);
    }
  },
};
